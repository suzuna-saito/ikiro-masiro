// インクルードガード
// ヘッダーファイルの読み込みの重複を防ぐ
#pragma once

// Object構造体の前方宣言
struct Object;

//---------------------------------------
//	おばあちゃん関連の変数、関数の「宣言」
//  (あくまでも実装はcppファイル)
//---------------------------------------

// 変数(使用宣言)-------------------------
extern Object	grandmother;		// おばあちゃんデータ



// 関数宣言（プロトタイプ宣言）------------
// こういう関数が使われますよという定義

// おばあちゃんの初期化
void InitializeGrandmother();

// 二回目以降のおばあちゃんの初期化
void SecondInitializeIsGrandmother();

//おばあちゃん更新
void UpdateGrandmother();

// おばあちゃんの描画
void DrawGrandmother();

//おばあちゃんの当たり判定
void CheckHitGrandmother();