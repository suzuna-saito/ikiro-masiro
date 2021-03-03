// インクルードガード
// ヘッダーファイルの読み込みの重複を防ぐ
#pragma once

// Object構造体の前方宣言
struct Object;

//---------------------------------------
//	エネミー関連の変数、関数の「宣言」
//  (あくまでも実装はcppファイル)
//---------------------------------------

// 変数(使用宣言)-------------------------
extern Object enemy[50];		// エネミーデータ
extern Object flyenemy[50];		// フライエネミーデータ

// 関数宣言（プロトタイプ宣言）------------
// こういう関数が使われますよという定義

// エネミーの初期化
void InitializeEnemy(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY);
void InitializeFlayEnemy(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY);

//2週目以降の初期化用関数
void SecondInitialize(int _index);
void SecondInitializeIsFlyEnemy(int _index);
// エネミーの更新
void UpdateEnemy(int _index);
void UpdateFlayEnemy(int _index);

// エネミーの描画
void DrawEnemy(int _index);
void DrawFlayEnemy(int _index);

// エネミーの押し戻し
void PushBackEnemy(int _index);
void PushBackFlyEnemy(int _index);
