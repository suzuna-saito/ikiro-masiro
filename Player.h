// インクルードガード
// ヘッダーファイルの読み込みの重複を防ぐ
#pragma once

// Object構造体の前方宣言
struct Object;

//---------------------------------------
//	プレイヤー関連の変数、関数の「宣言」
//  (あくまでも実装はcppファイル)
//---------------------------------------

// 変数(使用宣言)-------------------------
extern Object player;		// プレイヤーデータ

// 変数
const float FIRST_JUMP_POWER = 7.5f;

// 関数宣言（プロトタイプ宣言）------------
// こういう関数が使われますよという定義

// プレイヤーの初期化
void InitializePlayer();

//2週目以降の初期化用関数
void SecondInitializeIsPlayer();


// プレイヤーの更新
void UpdatePlayer();

// プレイヤーの描画
void DrawPlayer();

//プレイヤーの当たり判定
void CheckHitPlayer(int _index);
void CheckHitFlyEnemyToPlayer(int _index);

// プレイヤーの押し戻し
void PushBackPlayer();

// プレイヤーと敵が当たったか?
void HitPlayerToEnemy();