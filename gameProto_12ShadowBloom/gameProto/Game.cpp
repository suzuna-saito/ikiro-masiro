#include "game.h"
#include "Input.h"
#include "renderer.h"
#include "SceneBase.h"
#include "Actor.h"
#include "Animation.h"
#include "Skeleton.h"
#include "CameraActor.h"
#include "PhysicsWorld.h"
#include "AudioManager.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <typeinfo>
#include <iostream>

Game::Game()
	:mRenderer(nullptr)
	, mNowScene(nullptr)
	, mIsRunning(true)
	, mIsPauseMode(false)
	, mActiveCamera(nullptr)
	, mPhysicsWorld(nullptr)
{
}

Game::~Game()
{
}

void Game::Input()
{
	// 終了イベントのキャッチ
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			// パッドイベント取得
			INPUT_INSTANCE.GetControllerInstance()->ReceiveEvent(event);
		}
	}

	// キーボード入力更新
	INPUT_INSTANCE.Update();

	// ESCが押されたら終了
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_ESCAPE))
	{
		mIsRunning = false;
	}

	// アクターデバッグ
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F12))
	{
		ShowActor();
	}

	// レンダリングリソース表示
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F11))
	{
		GetRenderer()->ShowResource();
	}

	// ポーズモード移行／解除
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_BACKSPACE))
	{
		//ポーズモード切替
		mIsPauseMode = !mIsPauseMode;
		Actor::State changeState;
		if (mIsPauseMode)
		{
			changeState = Actor::EPaused; //ポーズ
		}
		else
		{
			changeState = Actor::EActive; //アクティブ
		}

		//全てのステートを変更する
		for (auto itr : mActors)
		{
			if (itr->GetState() != Actor::EDead)
			{
				itr->SetState(changeState);
			}
		}
	}
}

void Game::Draw()
{
	if (!mNowScene)
	{
		return;
	}
	mNowScene->draw();
}

int Game::Update()
{
	// 16ミリ秒(= 60フレーム/秒)になるように、前のフレームからの16ミリ秒以上経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// 前のフレームから現在時刻までの経過時間算出(秒単位）
	mDeltaTime = (SDL_GetTicks() - mTicksCount + 16) / 1000.0f;
	//mDeltaTime = 8 / 1000.0f;

	//このフレームの開始時間保存（次のフレームまでの経過時間をはかるため）
	mTicksCount = SDL_GetTicks();

	// フレーム時間があまりにも経過している場合は0.05 → 20fps固定
	if (mDeltaTime > 0.05f)
	{
		mDeltaTime = 0.05f;
	}

	if (!mNowScene)
	{
		return 0;
	}
	SceneBase* tmpScene;

	// アクターの処理
	ActorUpdate();

	//あたり判定
	mPhysicsWorld->Collision();
	
	if (mActiveCamera != nullptr)
	{
		// カメラ行列計算
		mActiveCamera->Update(mDeltaTime);
		// アクティブカメラのビュー行列を持ってくる
		mViewMatrix = mActiveCamera->GetViewMatrix();
		GetRenderer()->SetViewMatrix(mViewMatrix);
	}
	//シーンを実行
	tmpScene = mNowScene->update();

	if (tmpScene != mNowScene)
	{
		delete mNowScene;

		//新規シーンを今のシーンに差し替え
		mNowScene = tmpScene;
		return 1;
	}
	return 0;
}

void Game::ActorUpdate()
{
	//全てのアクターの更新
	for (auto actor : mActors)
	{
		actor->Update(mDeltaTime);
	}

	//ペンディング中のアクターをアクティブアクターに移動
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 全ての死んでいるアクターを一時保管
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
	// 死んでいるアクターをdelete(mActorからも消去)
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();
}

void Game::ShowActor()
{
	printf("\n\n<--------------ActorList----------------->\n");
	printf("---------> Active Actor ( %zd ) <-----------\n", mActors.size());

	for (auto i : mActors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
	printf("---------> Pending Actor ( %zd ) <-----------\n", mPendingActors.size());
	for (auto i : mPendingActors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
}


void Game::Run()
{
	// レンダラーが初期化されていなかったら初期化
	if (!mRenderer)
	{
		Initialize(1024,768);
	}
	// 開始シーンが定義されていなかったら終了
	if (!mNowScene)
	{
		printf("開始シーンが設定されていません");
		return;
	}

	// ゲームループ
	while (mIsRunning)
	{
		Input();
		if (Update())
		{
			continue; // シーン切り替え時のみ描画処理をさせないため
		}
		Draw();
	}
}

void Game::Shutdown()
{
	// アクターの削除　（アクターを通じてコンポーネントも削除される）
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	while (!mPendingActors.empty())
	{
		delete mPendingActors.back();
	}
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
}

bool Game::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL初期化失敗: %s\n", SDL_GetError());
		return false;
	}

	// レンダラーの作成
	mRenderer = new Renderer;
	if (!mRenderer->Initialize(screenWidth, screenHeight, fullScreen))
	{
		printf("Rendererの初期化に失敗");
		delete mRenderer;
		return false;
	}

	// サウンドの初期化
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		delete mRenderer;
		return false;
	}
	printf("SDLMixer初期化完了\n");
	// SDLMixer API初期化　44100:音源の周波数 2:ステレオ 4096:内部バッファサイズ
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		delete mRenderer;
		Mix_CloseAudio();
		Mix_Quit();
		return false;
	}
	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}
	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI初期化完了\n");

	// AudioSystem作成
	mAudio = new AudioManager;

	// あたり判定システム作成
	mPhysicsWorld = new PhysicsWorld;

	// コントローラ初期化
	INPUT_INSTANCE.ControllerInit();

	mTicksCount = SDL_GetTicks();
	return true;
}

SDL_Renderer* Game::GetSDLRenderer()
{
	return mRenderer->GetSDLRenderer();
}

void Game::AddActor(Actor * actor)
{
	// いったんペンディングアクターに追加
	mPendingActors.emplace_back(actor);
}

void Game::RemoveActor(Actor * actor)
{
	//ペンディングアクター内にいる？
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//ペンディングアクターの最後尾にデータを移動して、データを消す
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
		return;
	}

	// アクティブアクター内にいる？
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//アクティブアクターの最後尾にデータを移動して、データ消す
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();

	}
}

void Game::SetCameraActor(CameraActor *camera)
{
	printf("SetCamera [%p]\n", camera);
	mActiveCamera = camera;
}

void Game::InActiveCamera(CameraActor * inActiveCam)
{
	if (inActiveCam == mActiveCamera)
	{
		printf("Camera is inActive. change to default view.\n");
		mActiveCamera = nullptr;
	}

	mViewMatrix = 
		Matrix4::CreateLookAt(Vector3(0, 0, 100),
		Vector3(0, 0, 0),
		Vector3(0, 0, 1));
}

void Game::SetPlayerActor(Actor* player)
{
	mPlayerActor = player;
}

const Matrix4 & Game::GetViewMatrix()
{
	return mViewMatrix;
}

const Vector3 & Game::GetViewTarget()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");		
	}
	return mActiveCamera->GetViewTarget();
}

const Vector3& Game::GetViewPos()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewPos();
}

