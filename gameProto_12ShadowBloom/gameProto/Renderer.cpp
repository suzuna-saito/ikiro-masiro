#include "Renderer.h"
#include "Game.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "DepthMap.h"
#include "HDRRenderer.h"

Renderer::Renderer()
	:mWindow(nullptr)
	,mSDLRenderer(nullptr)
	,mContext(0)
	,mMeshShader(nullptr)
	,mSkinnedShader(nullptr)
	,mDepthMapRender(nullptr)
	,mHDRRenderer(nullptr)
	,mUndefineTexID(0)
{
}

Renderer::~Renderer()
{
	delete mDepthMapRender;
	delete mHDRRenderer;
}

bool Renderer::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	mScreenWidth  = screenWidth;
	mScreenHeight = screenHeight;

	// OpenGL アトリビュートのセット
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// GL version 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// 8Bit RGBA チャンネル
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE  , 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE , 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// ダブルバッファリング
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを強制
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Windowの作成
	mWindow = SDL_CreateWindow("SDL & GL Window",
		                       100, 80,
		                       mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if (!mWindow)
	{
		printf("Windowの作成に失敗: %s", SDL_GetError());
		return false;
	}
	if (fullScreen)
	{
		if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d, %d) サイズのフルスクリーン化に失敗\n", screenWidth, screenHeight);
			return false;
		}
		glViewport(0, 0, mScreenWidth, mScreenHeight);
	}

	//SDLRendererの作成
	mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mSDLRenderer)
	{
		printf("SDLRendererの作成に失敗 : %s", SDL_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNGの初期化に失敗 : %s", SDL_GetError());
		return false;
	}

	// OpenGLContextの作成
	mContext = SDL_GL_CreateContext(mWindow);

	// Glewの初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEWの初期化に失敗");
		return false;
	}
	// 幾つかのプラットホームでは、GLEWが無害なエラーコードを吐くのでクリアしておく
	glGetError();

	// シェーダープログラムの初期化
	if (!LoadShaders())
	{
		printf("シェーダーの初期化に失敗");
		return false;
	}
	// 未定義テクスチャ
	mUndefineTexID = GetTexture("assets/noneTexture.png")->GetTextureID();

	// デプスレンダラー初期化
	mDepthMapRender = new DepthMap;
	const int depthmapSize = 2048;
	mDepthMapRender->CreateShadowMap(depthmapSize);

	// HDRレンダラー初期化
	mHDRRenderer = new HDRRenderer(mScreenWidth, mScreenHeight, 4);

	// カリング
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	// 2D テクスチャ用頂点初期化
	CreateSpriteVerts();

	return true;
}

void Renderer::SetDepthSetting(const Vector3& centerWorldPos, const Vector3& lightDir, const Vector3& upVec, float lightDistance)
{
	mDepthMapRender->CalcLightSpaceMatrix(centerWorldPos, lightDir, upVec, lightDistance);
}

void Renderer::Shutdown()
{
	//テクスチャ破棄
	for (auto i : mTextures)
	{
		printf("Texture Release : %s\n", i.first.c_str());
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// メッシュ破棄
	for (auto i : mMeshs)
	{
		printf("Mesh Release : %s\n", i.first.c_str());
		i.second->Unload();
		delete i.second;
	}
	mMeshs.clear();

	//シェーダー破棄
	mMeshShader->Unload();

	// スケルトンの破棄
	for (auto s : mSkeletons)
	{
		delete s.second;
	}

	// アニメーションの破棄
	for (auto a : mAnims)
	{
		delete a.second;
	}

	// SDL系の破棄
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	///////////////////////////////////////////////////////////////////////////////
	// デプスレンダリングパス
	///////////////////////////////////////////////////////////////////////////////
	Matrix4 lightSpaceMat = mDepthMapRender->GetLightSpaceMatrix();
	mDepthMapRender->DepthRenderingBegin();
	{
		// スタティックメッシュを描画
		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(mDepthMapRender->GetDepthMapShader());
			}
		}

		// スキンメッシュを描画
		mSkinnedDepthShader->SetActive();
		mSkinnedDepthShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		// スキンシェーダにパラメータセット
		for (auto sk : mSkeletalMeshes)
		{

			if (sk->GetVisible())
			{
				sk->Draw(mSkinnedDepthShader);
			}
		}
	}
	mDepthMapRender->DepthRenderignEnd(); // デプスレンダリングパス終了

	//////////////////////////////////////////////////////////////////
	// シャドウ + HDR
	//////////////////////////////////////////////////////////////////

	mHDRRenderer->HdrRecordBegin();
	{
		//シャドウ付きでメッシュを描画
		mMeshShadowHDRShader->SetActive();
		mMeshShadowHDRShader->SetMatrixUniform("uViewProj", mView * mProjection);
		mMeshShadowHDRShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		mMeshShadowHDRShader->SetIntUniform("uTexture", 0);

		// デプスマップをセット
		mMeshShadowHDRShader->SetIntUniform("depthMap", 4);// ライティング変数をセット
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, mDepthMapRender->GetDepthTexID());

		SetLightUniforms(mMeshShadowHDRShader);

		// 全てのメッシュコンポーネントを描画
		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(mMeshShadowHDRShader);
			}
		}

		// スキン＆シャドウメッシュ
		mSkinnedShadowHDRShader->SetActive();
		// ビュープロジェクション＆ライト空間行列セット
		mSkinnedShadowHDRShader->SetMatrixUniform("uViewProj", mView * mProjection);
		mSkinnedShadowHDRShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		mSkinnedShadowHDRShader->SetIntUniform("uTexture", 0);
		mSkinnedShadowHDRShader->SetIntUniform("depthMap", 4);

		// ライトパラメータのセット
		SetLightUniforms(mSkinnedShadowHDRShader);

		for (auto sk : mSkeletalMeshes)
		{
			if (sk->GetVisible())
			{
				sk->Draw(mSkinnedShadowHDRShader);
			}
		}
	}
	mHDRRenderer->HdrRecordEnd();

	//////////////////////////////////////////////////////////////////
	// HDR Bloom の作成＆合成
	/////////////////////////////////////////////////////////////////
	mHDRRenderer->HiBrightBlurCreate();
	mHDRRenderer->HdrTonemapAndBrightBlurCombine();

	// HDRBufferにレンダリングしたときのDepth情報をスクリーンにコピー
	mHDRRenderer->CopyDepthToScreen();

	// 当たり判定デバッグBoxの表示
	GAMEINSTANCE.GetPhysics()->DebugShowBox();
}

Texture* Renderer::GetTexture(const std::string & fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	printf("Load Texture Success : %s \n", fileName.c_str());
	return tex;
}

Mesh* Renderer::GetMesh(const std::string & fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshs.find(fileName);

	if (iter != mMeshs.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh;
		if (m->Load(fileName,this))
		{
			mMeshs.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	printf("Load Mesh Success : %s\n", fileName.c_str());
	return m;
}

void Renderer::AddMeshComponent(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshes.emplace_back(sk);
	}
	else
	{
		mMeshComponents.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComponent(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(iter);
	}
	else
	{
		auto iter = std::find(mMeshComponents.begin(), mMeshComponents.end(), mesh);
		mMeshComponents.erase(iter);
	}

}
void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		 0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		 0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}


void Renderer::ShowResource()
{
	printf("\n\n<------------------ textures ------------------>\n");
	for (auto i : mTextures)
	{
		printf("texfile %s\n", i.first.c_str());
	}

	printf("\n<------------------  meshes  ------------------->\n");
	for (auto i : mMeshs)
	{
		printf("meshfile %s\n", i.first.c_str());
	}
}

void Renderer::SetWindowTitle(const std::string & title)
{
	SDL_SetWindowTitle(mWindow, title.c_str());
}

void Renderer::SpriteDrawBegin()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mTilemapShader->SetActive();
	mSpriteVerts->SetActive();
}

void Renderer::SpriteDrawEnd()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

// テクスチャの描画
void Renderer::DrawTexture(class Texture* texture, int index, int xDivNum, int yDivNum, const Vector2& offset, float scale, float alpha)
{
	// テクスチャの幅・高さでスケーリング
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth() / xDivNum) * scale,
		static_cast<float>(texture->GetHeight() / yDivNum) * scale,
		1.0f);
	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3( offset.x - (mScreenWidth * 0.5f),
			    (mScreenHeight * 0.5f) - offset.y, 0.0f));
	// ワールド変換
	Vector2 tileSplitNum(static_cast<float>(xDivNum), static_cast<float>(yDivNum));
	Matrix4 world = scaleMat * transMat;
	mTilemapShader->SetMatrixUniform("uWorldTransform", world);
	mTilemapShader->SetIntUniform("uTileIndex", index);
	mTilemapShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	mTilemapShader->SetFloatUniform("uAlpha", alpha);
	// テクスチャセット
	texture->SetActive();

	// 四角形描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawTexture(Texture* texture, const Vector2& offset, float scale, float alpha)
{
	DrawTexture(texture, 0, 1, 1, offset, scale, alpha);
}

bool Renderer::LoadShaders()
{
	// スプライトシェーダーのロード
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// ビュープロジェクション行列のセット
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight));
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	mTilemapShader = new Shader();
	if (!mTilemapShader->Load("Shaders/Sprite.vert", "Shaders/Tilemap.frag"))
	{
		return false;
	}
	mTilemapShader->SetActive();
	// ビュープロジェクション行列のセット
	mTilemapShader->SetMatrixUniform("uViewProj", viewProj);

	// メッシュシェーダー
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

	mMeshShader->SetActive();
	mView       = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		                                        static_cast<float>(mScreenWidth),
		                                        static_cast<float>(mScreenHeight),
		                                        1.0f, 10000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// スキンメッシュシェーダー
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
	{
		return false;
	}
	mSkinnedShader->SetActive();
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);


	// デプススキンメッシュシェーダー
	mSkinnedDepthShader = new Shader();
	if (!mSkinnedDepthShader->Load("Shaders/SkinnedDepth.vert", "Shaders/Depthmap.frag"))
	{
		return false;
	}

	// 通常メッシュシャドウシェーダー
	mMeshShadowHDRShader = new Shader();
	if (!mMeshShadowHDRShader->Load("Shaders/ShadowMesh.vert", "Shaders/ShadowHDRMesh.frag"))
	{
		return false;
	}

	// スキンシャドウシェーダー
	mSkinnedShadowHDRShader = new Shader;
	if (!mSkinnedShadowHDRShader->Load("Shaders/SkinnedShadow.vert", "Shaders/ShadowHDRMesh.frag"))
	{
		return false;
	}

	return true;
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// ビュー行列からカメラ位置を逆算出する
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	//アンビエントライト
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	//ディレクショナルライト
	shader->SetVectorUniform("uDirLight.mDirection", mDirectionalLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirectionalLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirectionalLight.mSpecColor);
}

//////////////////////////////////////////////////////////////
// スケルタル情報の取得
// in  : スケルタル情報 .gpskelファイル名
// out : Skeleton情報へのポインタ
// Desc: gpskelファイル名より、スケルタル情報を返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpskelファイル名をキーとするスケルタル情報のmapが作成される
//////////////////////////////////////////////////////////////
const Skeleton* Renderer::GetSkeleton(const char* fileName)
{
	std::string file(fileName);
	auto iter = mSkeletons.find(file);
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(file))
		{
			mSkeletons.emplace(file, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

//////////////////////////////////////////////////////////////
// アニメーション情報の取得
// in  : アニメーションデータが格納されている .gpanimファイル名
// out : アニメーション情報へのポインタ
// Desc: gpanimファイル名よりアニメーションデータを返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpanimファイル名をキーとするアニメーション情報のmapが作成される
//////////////////////////////////////////////////////////////
const Animation* Renderer::GetAnimation(const char* fileName, bool loop)
{
	auto iter = mAnims.find(fileName);
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName, loop))
		{
			mAnims.emplace(fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}


bool GLErrorHandle(const char* location)
{
	GLenum error_code = glGetError();
	if (error_code == GL_NO_ERROR)
	{
		return true;
	}
	do
	{
		const char* msg;
		switch (error_code)
		{
		case GL_INVALID_ENUM:                  msg = "INVALID_ENUM"     ; break;
		case GL_INVALID_VALUE:                 msg = "INVALID_VALUE"    ; break;
		case GL_INVALID_OPERATION:             msg = "INVALID_OPERATION"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: msg = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: msg = "unknown Error";
		}
		printf("GLErrorLayer: ERROR%04x'%s' location: %s\n", error_code, msg, location);
		error_code = glGetError();
	} while (error_code != GL_NO_ERROR);

	return false;
}
