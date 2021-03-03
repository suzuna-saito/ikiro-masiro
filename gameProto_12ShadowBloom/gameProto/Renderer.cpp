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

	// OpenGL �A�g���r���[�g�̃Z�b�g
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// GL version 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// 8Bit RGBA �`�����l��
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE  , 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE , 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// �_�u���o�b�t�@�����O
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// �n�[�h�E�F�A�A�N�Z�����[�V����������
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Window�̍쐬
	mWindow = SDL_CreateWindow("SDL & GL Window",
		                       100, 80,
		                       mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if (!mWindow)
	{
		printf("Window�̍쐬�Ɏ��s: %s", SDL_GetError());
		return false;
	}
	if (fullScreen)
	{
		if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d, %d) �T�C�Y�̃t���X�N���[�����Ɏ��s\n", screenWidth, screenHeight);
			return false;
		}
		glViewport(0, 0, mScreenWidth, mScreenHeight);
	}

	//SDLRenderer�̍쐬
	mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mSDLRenderer)
	{
		printf("SDLRenderer�̍쐬�Ɏ��s : %s", SDL_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNG�̏������Ɏ��s : %s", SDL_GetError());
		return false;
	}

	// OpenGLContext�̍쐬
	mContext = SDL_GL_CreateContext(mWindow);

	// Glew�̏�����
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW�̏������Ɏ��s");
		return false;
	}
	// ����̃v���b�g�z�[���ł́AGLEW�����Q�ȃG���[�R�[�h��f���̂ŃN���A���Ă���
	glGetError();

	// �V�F�[�_�[�v���O�����̏�����
	if (!LoadShaders())
	{
		printf("�V�F�[�_�[�̏������Ɏ��s");
		return false;
	}
	// ����`�e�N�X�`��
	mUndefineTexID = GetTexture("assets/noneTexture.png")->GetTextureID();

	// �f�v�X�����_���[������
	mDepthMapRender = new DepthMap;
	const int depthmapSize = 2048;
	mDepthMapRender->CreateShadowMap(depthmapSize);

	// HDR�����_���[������
	mHDRRenderer = new HDRRenderer(mScreenWidth, mScreenHeight, 4);

	// �J�����O
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	// 2D �e�N�X�`���p���_������
	CreateSpriteVerts();

	return true;
}

void Renderer::SetDepthSetting(const Vector3& centerWorldPos, const Vector3& lightDir, const Vector3& upVec, float lightDistance)
{
	mDepthMapRender->CalcLightSpaceMatrix(centerWorldPos, lightDir, upVec, lightDistance);
}

void Renderer::Shutdown()
{
	//�e�N�X�`���j��
	for (auto i : mTextures)
	{
		printf("Texture Release : %s\n", i.first.c_str());
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// ���b�V���j��
	for (auto i : mMeshs)
	{
		printf("Mesh Release : %s\n", i.first.c_str());
		i.second->Unload();
		delete i.second;
	}
	mMeshs.clear();

	//�V�F�[�_�[�j��
	mMeshShader->Unload();

	// �X�P���g���̔j��
	for (auto s : mSkeletons)
	{
		delete s.second;
	}

	// �A�j���[�V�����̔j��
	for (auto a : mAnims)
	{
		delete a.second;
	}

	// SDL�n�̔j��
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	///////////////////////////////////////////////////////////////////////////////
	// �f�v�X�����_�����O�p�X
	///////////////////////////////////////////////////////////////////////////////
	Matrix4 lightSpaceMat = mDepthMapRender->GetLightSpaceMatrix();
	mDepthMapRender->DepthRenderingBegin();
	{
		// �X�^�e�B�b�N���b�V����`��
		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(mDepthMapRender->GetDepthMapShader());
			}
		}

		// �X�L�����b�V����`��
		mSkinnedDepthShader->SetActive();
		mSkinnedDepthShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		// �X�L���V�F�[�_�Ƀp�����[�^�Z�b�g
		for (auto sk : mSkeletalMeshes)
		{

			if (sk->GetVisible())
			{
				sk->Draw(mSkinnedDepthShader);
			}
		}
	}
	mDepthMapRender->DepthRenderignEnd(); // �f�v�X�����_�����O�p�X�I��

	//////////////////////////////////////////////////////////////////
	// �V���h�E + HDR
	//////////////////////////////////////////////////////////////////

	mHDRRenderer->HdrRecordBegin();
	{
		//�V���h�E�t���Ń��b�V����`��
		mMeshShadowHDRShader->SetActive();
		mMeshShadowHDRShader->SetMatrixUniform("uViewProj", mView * mProjection);
		mMeshShadowHDRShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		mMeshShadowHDRShader->SetIntUniform("uTexture", 0);

		// �f�v�X�}�b�v���Z�b�g
		mMeshShadowHDRShader->SetIntUniform("depthMap", 4);// ���C�e�B���O�ϐ����Z�b�g
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, mDepthMapRender->GetDepthTexID());

		SetLightUniforms(mMeshShadowHDRShader);

		// �S�Ẵ��b�V���R���|�[�l���g��`��
		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(mMeshShadowHDRShader);
			}
		}

		// �X�L�����V���h�E���b�V��
		mSkinnedShadowHDRShader->SetActive();
		// �r���[�v���W�F�N�V���������C�g��ԍs��Z�b�g
		mSkinnedShadowHDRShader->SetMatrixUniform("uViewProj", mView * mProjection);
		mSkinnedShadowHDRShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		mSkinnedShadowHDRShader->SetIntUniform("uTexture", 0);
		mSkinnedShadowHDRShader->SetIntUniform("depthMap", 4);

		// ���C�g�p�����[�^�̃Z�b�g
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
	// HDR Bloom �̍쐬������
	/////////////////////////////////////////////////////////////////
	mHDRRenderer->HiBrightBlurCreate();
	mHDRRenderer->HdrTonemapAndBrightBlurCombine();

	// HDRBuffer�Ƀ����_�����O�����Ƃ���Depth�����X�N���[���ɃR�s�[
	mHDRRenderer->CopyDepthToScreen();

	// �����蔻��f�o�b�OBox�̕\��
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

// �e�N�X�`���̕`��
void Renderer::DrawTexture(class Texture* texture, int index, int xDivNum, int yDivNum, const Vector2& offset, float scale, float alpha)
{
	// �e�N�X�`���̕��E�����ŃX�P�[�����O
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth() / xDivNum) * scale,
		static_cast<float>(texture->GetHeight() / yDivNum) * scale,
		1.0f);
	// �X�N���[���ʒu�̕��s�ړ�
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3( offset.x - (mScreenWidth * 0.5f),
			    (mScreenHeight * 0.5f) - offset.y, 0.0f));
	// ���[���h�ϊ�
	Vector2 tileSplitNum(static_cast<float>(xDivNum), static_cast<float>(yDivNum));
	Matrix4 world = scaleMat * transMat;
	mTilemapShader->SetMatrixUniform("uWorldTransform", world);
	mTilemapShader->SetIntUniform("uTileIndex", index);
	mTilemapShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	mTilemapShader->SetFloatUniform("uAlpha", alpha);
	// �e�N�X�`���Z�b�g
	texture->SetActive();

	// �l�p�`�`��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawTexture(Texture* texture, const Vector2& offset, float scale, float alpha)
{
	DrawTexture(texture, 0, 1, 1, offset, scale, alpha);
}

bool Renderer::LoadShaders()
{
	// �X�v���C�g�V�F�[�_�[�̃��[�h
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// �r���[�v���W�F�N�V�����s��̃Z�b�g
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight));
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	mTilemapShader = new Shader();
	if (!mTilemapShader->Load("Shaders/Sprite.vert", "Shaders/Tilemap.frag"))
	{
		return false;
	}
	mTilemapShader->SetActive();
	// �r���[�v���W�F�N�V�����s��̃Z�b�g
	mTilemapShader->SetMatrixUniform("uViewProj", viewProj);

	// ���b�V���V�F�[�_�[
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

	// �X�L�����b�V���V�F�[�_�[
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
	{
		return false;
	}
	mSkinnedShader->SetActive();
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);


	// �f�v�X�X�L�����b�V���V�F�[�_�[
	mSkinnedDepthShader = new Shader();
	if (!mSkinnedDepthShader->Load("Shaders/SkinnedDepth.vert", "Shaders/Depthmap.frag"))
	{
		return false;
	}

	// �ʏ탁�b�V���V���h�E�V�F�[�_�[
	mMeshShadowHDRShader = new Shader();
	if (!mMeshShadowHDRShader->Load("Shaders/ShadowMesh.vert", "Shaders/ShadowHDRMesh.frag"))
	{
		return false;
	}

	// �X�L���V���h�E�V�F�[�_�[
	mSkinnedShadowHDRShader = new Shader;
	if (!mSkinnedShadowHDRShader->Load("Shaders/SkinnedShadow.vert", "Shaders/ShadowHDRMesh.frag"))
	{
		return false;
	}

	return true;
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// �r���[�s�񂩂�J�����ʒu���t�Z�o����
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	//�A���r�G���g���C�g
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	//�f�B���N�V���i�����C�g
	shader->SetVectorUniform("uDirLight.mDirection", mDirectionalLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirectionalLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirectionalLight.mSpecColor);
}

//////////////////////////////////////////////////////////////
// �X�P���^�����̎擾
// in  : �X�P���^����� .gpskel�t�@�C����
// out : Skeleton���ւ̃|�C���^
// Desc: gpskel�t�@�C�������A�X�P���^������Ԃ��B�Ȃ��ꍇ�͂��̃t�@�C������
//       �ǂݍ��݂��s���B�ǂݍ��݂��s���Ă��t�@�C�������݂��Ȃ��ꍇ nullptr��Ԃ�
//       ������gpskel�t�@�C�������L�[�Ƃ���X�P���^������map���쐬�����
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
// �A�j���[�V�������̎擾
// in  : �A�j���[�V�����f�[�^���i�[����Ă��� .gpanim�t�@�C����
// out : �A�j���[�V�������ւ̃|�C���^
// Desc: gpanim�t�@�C�������A�j���[�V�����f�[�^��Ԃ��B�Ȃ��ꍇ�͂��̃t�@�C������
//       �ǂݍ��݂��s���B�ǂݍ��݂��s���Ă��t�@�C�������݂��Ȃ��ꍇ nullptr��Ԃ�
//       ������gpanim�t�@�C�������L�[�Ƃ���A�j���[�V��������map���쐬�����
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
