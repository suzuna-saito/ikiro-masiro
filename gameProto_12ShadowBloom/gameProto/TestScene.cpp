#include "game.h"
#include "Math.h"
#include "Renderer.h"
#include "TestScene.h"
#include "PlayerActor.h"
#include "DebugGrid.h"
#include "LookDownCamera.h"
#include "ThirdPersonCamera.h"
#include "AudioManager.h"
#include "LevelManager.h"
#include "BitmapText.h"
#include "Input.h"
#include "LevelActor.h"

#include "BlackKnightActor.h"

#pragma warning(disable:4996)

TestScene::TestScene()
{ 
	//プレーヤー
	PlayerActor *player = new PlayerActor;
	player->SetPosition(Vector3(250, 200, 100));
	GAMEINSTANCE.SetPlayerActor(player);


	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.5f, 0.5f, 0.5f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.8f, 0.8f, 0.8f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid( 1000.0f, 30, color );

	ThirdPersonCamera* c = new ThirdPersonCamera(player);
	c->Init(Vector3(300, 0, 300), Vector3(0, 0, 0), Vector3(0, 0, 1));
	c->SetCameraLength(800.0f);

	AUDIO->GetSound("assets/audio/jump.wav");
	AUDIO->GetSound("assets/audio/coin.wav");
	AUDIO->GetMusic("assets/audio/loopMusic.wav");
	//AUDIO->PlayMusic("assets/audio/loopMusic.wav");

	BlackKnightActor* enemy = new BlackKnightActor;
	enemy->SetPosition(Vector3(200,200,10));

	// レベル読み込み
	//mLevel = new LevelManager;
	LevelActor* level = new LevelActor;
	Vector3 offs(400, 20, 0);
	level->LoadLevel("assets/dungeon/MapStaticMesh.gpmesh", "assets/dungeon/collision.json",offs);
	
	mTexture = RENDERER->GetTexture("assets/texture.png");
	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "assets/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

}

TestScene::~TestScene()
{
	delete mLevel;
}

SceneBase *TestScene::update()
{
	float radius = 160.0f;
	static float anim = 0.0f;
	anim += 0.01f;
	Matrix4 view;
	view = GAMEINSTANCE.GetViewMatrix();
	GAMEINSTANCE.GetRenderer()->SetViewMatrix(view);

	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();
	}

	Vector3 playerPos = GAMEINSTANCE.GetPlayerActor()->GetPosition();

	// レンダラに登録されているディレクショナルライト方向を取得
	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	lightDir.Normalize();

	// シャドウマップレンダリングのためのライト情報渡す
	RENDERER->SetDepthSetting(playerPos, lightDir, Vector3::UnitZ, 5000.0f);

	return this;
}

void TestScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	//ゲームシステム関連描画
	GAMEINSTANCE.GetRenderer()->Draw();

	static float anim = 0.0f;
	anim += 0.01f;

	// 2D描画
	//RENDERER->SpriteDrawBegin();
	////RENDERER->DrawTexture(mTexture, 3, 2, 2, Vector2(100.0, 100.0), 1.0f, 1.0f);
	//RENDERER->DrawTexture(mTexture, Vector2(100.0, 100.0));
	//char buf[256];

	//sprintf(buf, "anim(%f)", anim);
	//mFont->TextDraw(200, 300, buf);

	//RENDERER->SpriteDrawEnd();

	//画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}