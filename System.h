#pragma once
//---------------------------------------
//	ゲーム全般で使う変数、関数の「宣言」
//---------------------------------------

// 定数 -----------------------------------------
const float PLAYER_SIZE = 32;
const float BLOCK_SIZE = 32;

const float ENEMY_SIZE = 32;

const float GRAVITY_ACCEL = 0.49f;
const float MAX_SPEED = 6.0f;

const int PLAYER_HP = 30;

const int TAMA_MAX = 3;
// 変数(使用宣言)-------------------------------------------
// キー入力情報
extern char keyState[256];		// 現在のキー入力情報
extern char preKeyState[256];	// １フレーム前のキー入力情報
struct Vector2
{
	float x;
	float y;
};

// 構造体 ----------------------------------------
// オブジェクトデータ
struct Object
{
	// 座標
	float posX;
	float posY;
	//初期化用ポジション
	float initPosX;
	float initPosY;

	Vector2 vector2;

	float drawPosX;
	float drawPosY;

	// サイズ
	float sizeX;
	float sizeY;

	// 速度
	float velocityX;
	float velocityY;

	//向き
	Vector2 direction;

	// 移動スピード
	float moveSpeed;
	//加速する力
	float accelePower;

	//画像
	int image;
	int image2;
	int DmgImage;
	int DmgImage2;
	int DmgImage3;
	int EndImage;
	int houseImage;

	//反転画像
	int Left_image;
	int Left_DmgImage;

	bool DrawFlag;
	bool DamageDrawFlag;

	//画像反転
	bool InversionFlag;

	//変化させない
	bool NotChangeFlag;

	//エネミー
	int intervalCount;
	int jumpCount;
	bool isJumpFlag;
	float jumpPower;
	//設置判定フラグ
	bool isGround;

	//入力フラグ
	bool inputFlag;
	//加速フラグ
	bool acceleFlag;

	bool isDamage;

	//弾
	bool ShotFlag;
	bool Left_ShotFlag;

	int count;

	//ダメージフラグ
	bool Dmg_Flag;

	//ダメージカウント
	int Dmg_count;

	//	HP
	int hitpoint;
	int hpCount;
	bool HitFlag;

};


// 関数宣言 ----------------------------------------
// ゲームの初期化
void Initialize();

// アクションゲームのメインループ
void ActionMain();

// 更新
void Update();
void UpdateInput();

// 描画
void Draw();

// 矩形の当たり判定
bool IsHit(Object obj1, Object obj2);

bool IsHit(float _posX1, float _posY1,
		   float _sizeX1, float _sizeY1,
		   float _posX2, float _posY2,
		   float _sizeX2, float _sizeY2);

//ゲームが始まっているかどうか

void HitUp(Object& obj);
void HitDown(Object& obj);


float Start(int obj);

