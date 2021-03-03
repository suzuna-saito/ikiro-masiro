#include "HDRRenderer.h"
#include "Renderer.h"
#include "Shader.h"

HDRRenderer::HDRRenderer(int fbowidth, int fboheight, int bloomLevel)
	: mGaussianBlurShader(nullptr)
	, mDownSamplingShader(nullptr)
	, mHdrToneAndBlurBlendShader(nullptr)
	, mQuadScreenVAO(0)
	, mQuadScreenVBO(0)
	, mHdrFBO(0)
	, mHdrRBO(0)
	, mBufferWidth(fbowidth)
	, mBufferHeight(fboheight)
	, mBloomBufferLevel(bloomLevel)
{
	// HDR バッファとBlur用バッファを作成
	InitHDRBuffers();
	InitBlurBuffers();
	// スクリーンバッファ描画用頂点作成
	InitScreenQuadVAO();

	mGaussianBlurShader = new Shader;
	if (!mGaussianBlurShader->Load("shaders/framebufferScreen.vert", "shaders/gaussianblur.frag"))
	{
		printf("GaussianBlurShader Load Failed.\n");
	}
	mDownSamplingShader = new Shader;
	if (!mDownSamplingShader->Load("shaders/framebufferScreen.vert", "shaders/downsampling.frag"))
	{
		printf("DownSamplingShader Load Failed.\n");
	}

	mHdrToneAndBlurBlendShader = new Shader;
	if(!mHdrToneAndBlurBlendShader->Load("shaders/framebufferScreen.vert", "shaders/hdrBlurBlend.frag"))
	{ 
		printf("HdrToneAndBlurBlendShader Load Failed\n");
	}
}

HDRRenderer::~HDRRenderer()
{
	delete mGaussianBlurShader;
	delete mDownSamplingShader;
	delete mHdrToneAndBlurBlendShader;
	for (auto item : mBlurFBOs)
	{
		glDeleteFramebuffers(1, &item);
	}
	for (auto item : mBlurBufferTexs)
	{
		glDeleteTextures(1, &item);
	}
	glDeleteBuffers(1, &mQuadScreenVBO);
	glDeleteVertexArrays(1, &this->mQuadScreenVAO);
}

void HDRRenderer::HdrRecordBegin()
{
	// 描画対象をmHdrFBOに変更( HDRcolor + HDRHighBrightの２枚へ出力)
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);
	glEnable(GL_DEPTH_TEST);

	// カラーバッファのクリア
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void HDRRenderer::HdrRecordEnd()
{
	// 描画対象をスクリーンへ戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRRenderer::HiBrightBlurCreate()
{
	glDisable(GL_DEPTH_TEST);
	unsigned int renderSource = mHdrColorBuffers[1];

	// 縮小バッファ1,3,5,7,9にコピー
	int reducex = mBufferWidth;
	int reducey = mBufferHeight;

	// ダウンサンプリング 1, 3, 5 ...と奇数番にダウンサンプリング結果出力
	for (unsigned int i = 0; i < mBloomBufferLevel; i++)
	{
		reducex /= 2;
		reducey /= 2;
		glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBOs[i * 2 + 1]);
		{
			glViewport(0, 0, reducex, reducey);
			//カラーバッファのクリア
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderSource);
			mDownSamplingShader->SetActive();
			mDownSamplingShader->SetIntUniform("blursource", 0);

			glBindVertexArray(mQuadScreenVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		renderSource = mBlurBufferTexs[i * 2 + 1];
	}

	// ガウスぼかし
	reducex = mBufferWidth;
	reducey = mBufferHeight;
	float deviation = 5.0f;
	renderSource = mBlurBufferTexs[1];

	// ガウスレベル数分
	for (unsigned int i = 0; i < mBloomBufferLevel; i++)
	{
		reducex /= 2;
		reducey /= 2;
		bool h = false;
		// horizontal 0 : 水平 1: 垂直方向 にガウスぼかしかける
		for (int horizontal = 0; horizontal < 2; horizontal++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBOs[i * 2 + horizontal]);
			{
				glViewport(0, 0, reducex, reducey);
				Vector2 dir;
				if (horizontal)
				{
					dir = Vector2(0, 1);
				}
				else
				{
					dir = Vector2(1, 0);
				}
				CalcGaussBlurParam(reducex, reducey, dir, deviation);

				// 回数ごとに偏差を上げる
				deviation *= deviation;

				// カラーバッファのクリア
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, renderSource);
				mGaussianBlurShader->SetActive();
				mGaussianBlurShader->SetIntUniform("blursource", 0);
				mGaussianBlurShader->SetIntUniform("param.SampleCount", mSampleCount);

				for (int i = 0; i < mSampleCount; i++)
				{
					std::string s = "param.Offset[" + std::to_string(i) + "]";
					mGaussianBlurShader->SetVectorUniform(s.c_str(), mOffset[i]);
				}

				glBindVertexArray(mQuadScreenVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				renderSource = mBlurBufferTexs[i * 2 + horizontal];
			}
		}
	}
	glEnable(GL_DEPTH_TEST);

}

void HDRRenderer::HdrTonemapAndBrightBlurCombine()
{
	// 元画像とガウスぼかし画像をすべて合成する
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_FRONT);
	{
		glViewport(0, 0, mBufferWidth, mBufferHeight);

		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mHdrColorBuffers[0]);

		for (unsigned int i = 0; i < mBloomBufferLevel; i++)
		{
			glActiveTexture(GL_TEXTURE1 + i );
			glBindTexture(GL_TEXTURE_2D, mBlurBufferTexs[i * 2 + 1]);
		}

		mHdrToneAndBlurBlendShader->SetActive();
		mHdrToneAndBlurBlendShader->SetIntUniform("scene", 0);
		mHdrToneAndBlurBlendShader->SetIntUniform("bloom1", 1);
		mHdrToneAndBlurBlendShader->SetIntUniform("bloom2", 2);
		mHdrToneAndBlurBlendShader->SetIntUniform("bloom3", 3);
		mHdrToneAndBlurBlendShader->SetIntUniform("bloom4", 4);
		mHdrToneAndBlurBlendShader->SetIntUniform("bloom5", 5);

		const float exposure = 0.8f;

		mHdrToneAndBlurBlendShader->SetFloatUniform("exposure", exposure);

		glBindVertexArray(mQuadScreenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	// Activeテクスチャにsetした内容を黒色テクスチャで無効にする
	unsigned int texNone = RENDERER->GetUndefineTexID();
	for (unsigned int i = 0; i < mBloomBufferLevel; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, texNone);
	}
}

void HDRRenderer::CopyDepthToScreen()
{
	// gBufferの深度をデフォルトの深度バッファにコピーする
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mHdrFBO);  // 読み込みバッファをgBufferに指定
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);        // 書き込みバッファをスクリーンに指定
	// 深度情報をスクリーンの深度バッファにコピー
	glBlitFramebuffer(
		0, 0, mBufferWidth, mBufferHeight, 0, 0, mBufferWidth, mBufferHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	// 通常のスクリーンへの描画に戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}

void HDRRenderer::InitHDRBuffers()
{
	/////////////////////////////////////////////////
	// 浮動小数点フレームバッファ（カラーバッファ２つ）の作成
	/////////////////////////////////////////////////
	glGenFramebuffers(1, &mHdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);
	{
		glGenTextures(2, mHdrColorBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_2D, mHdrColorBuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mBufferWidth, mBufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			// attach texture to framebuffer
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mHdrColorBuffers[i], 0
			);
		}
		// レンダーバッファーの作成
		glGenRenderbuffers(1, &mHdrRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, mHdrRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mBufferWidth, mBufferHeight);
		// FBOにレンダーバッファーをアタッチする
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mHdrRBO);

		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf( "HDRRenderer ERROR::FRAMEBUFFER:: Framebuffer is not complete! \n");
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // デフォルトに戻す
}

void HDRRenderer::InitBlurBuffers()
{
	int w = mBufferWidth;
	int h = mBufferHeight;

	float borderColor[4] = { 0.0f };
	mBlurFBOs.resize(mBloomBufferLevel * 2);
	mBlurBufferTexs.resize(mBloomBufferLevel * 2);

	// 横方向・縦方向ブラー作成　mBloomBufferLevelの２倍分の枚数作成
	for (unsigned int i = 0; i < mBloomBufferLevel; i++)
	{
		// 縮小バッファ幅・高さ
		w /= 2;
		h /= 2;
		// j : 0 横方向 1 : 縦方向
		for (int j = 0; j < 2; j++)
		{
			glGenFramebuffers(1, &mBlurFBOs[i * 2 + j]);
			glGenTextures(1, &mBlurBufferTexs[i * 2 + j]);
			{
				glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBOs[i * 2 + j]);
				glBindTexture(GL_TEXTURE_2D, mBlurBufferTexs[i * 2 + j]);
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL
				);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glFramebufferTexture2D(
					GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurBufferTexs[i * 2 + j], 0
				);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}

void HDRRenderer::InitScreenQuadVAO()
{
	// 画面全体書く用のVAO作成
	// スクリーン全体を描く四角形用頂点配列
	float quadVertices[] = {
		// ポジション   // テクスチャ座標
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// スクリーン全体を描く四角形用 VAO
	glGenVertexArrays(1, &mQuadScreenVAO);
	glGenBuffers(1, &mQuadScreenVBO);
	glBindVertexArray(mQuadScreenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

float HDRRenderer::GaussianDistribution(const Vector2& pos, float rho)
{
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}

void HDRRenderer::CalcGaussBlurParam(int w, int h, Vector2 dir, float deviation)
{
	auto tu = 1.0f / float(w);
	auto tv = 1.0f / float(h);

	mOffset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), deviation);
	auto total_weight = mOffset[0].z;

	mOffset[0].x = 0.0f;
	mOffset[0].y = 0.0f;

	for (auto i = 1; i < 8; ++i)
	{
		int nextpos = (i - 1) * 2 + 1;
		mOffset[i].x = dir.x * tu * nextpos;
		mOffset[i].y = dir.y * tv * nextpos;
		mOffset[i].z = GaussianDistribution(dir * float(nextpos), deviation);
		total_weight += mOffset[i].z * 2.0f;
	}
	for (auto i = 0; i < 8; ++i)
	{
		mOffset[i].z /= total_weight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		mOffset[i].x = -mOffset[i - 7].x;
		mOffset[i].y = -mOffset[i - 7].y;
		mOffset[i].z = mOffset[i - 7].z;
	}
}
