#pragma once
#include "Block.h"
//---------------------------------------
//	ブロック関連
//---------------------------------------
struct Object;

// 変数(使用宣言)-------------------------
extern Object block[];		// ブロックデータ

// 関数宣言（プロトタイプ宣言）------------
// こういう関数が使われますよという定義

// ブロックの初期化
void InitializeBlock(int _index, int _image, float _x, float _y, float _sizeX, float _sizeY);

// ブロックの更新
void UpdateBlock(int _index);

// ブロックの描画
void DrawBlock(int _index);
