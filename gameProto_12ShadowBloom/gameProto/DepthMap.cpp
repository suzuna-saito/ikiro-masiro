#include "DepthMap.h"
#include "depthMap.h"
#include "Renderer.h"
#include "shader.h"

DepthMap::DepthMap()
	: mDepthShader(nullptr)
	, mDepthMapSize(0)
	, mDepthMapFBO(0)
	, mDepthMap(0)
{
	mDepthShader = new Shader;
	mDepthShader->Load("shaders/Depthmap.vert", "shaders/Depthmap.frag");

}

DepthMap::~DepthMap()
{
	delete mDepthShader;
	glDeleteFramebuffers(1, &mDepthMapFBO);
	glDeleteTextures(1, &mDepthMap);
}

// デプスマップの描画を開始する 
void DepthMap::DepthRenderingBegin()
{
	// 描画先をデプスマップに設定しシェーダーをセットする
	glViewport(0, 0, mDepthMapSize, mDepthMapSize);
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	mDepthShader->SetActive();
	mDepthShader->SetMatrixUniform("lightSpaceMatrix", mLightSpaceMatrix);
}

// デプスマップの描画を終了する
void DepthMap::DepthRenderignEnd()
{
	// 描画先をスクリーンに戻す
	glViewport(0, 0,
		static_cast<GLsizei>(RENDERER->GetScreenWidth()),
		static_cast<GLsizei>(RENDERER->GetScreenHeight()));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
}

// シャドウマップを作成する
void DepthMap::CreateShadowMap(unsigned int depthSIze)
{
	mDepthMapSize = depthSIze;
	glGenFramebuffers(1, &mDepthMapFBO);

	glGenTextures(1, &mDepthMap);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mDepthMapSize, mDepthMapSize,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// ライト空間行列を計算する
void DepthMap::CalcLightSpaceMatrix(const Vector3& centerWorldPos, const Vector3& lightDir, const Vector3& upVec, float lightDistance)
{
	Vector3 viewPos;

	mLightDir = lightDir;
	mLightOrigin = (-lightDistance) * mLightDir + centerWorldPos;

	Matrix4 projection = Matrix4::CreateOrtho(3000, 3000, 1.0f, 10000.0f);
	Matrix4 lightView = Matrix4::CreateLookAt(mLightOrigin, centerWorldPos, upVec);

	mLightSpaceMatrix = lightView * projection;
}
