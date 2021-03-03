#pragma once

// Object構造体の前方宣言
struct Object;

// 変数(使用宣言)-------------------------
extern Object hitPoint[];		// プレイヤーデータ

// プレイヤーの初期化
void InitializeHP();

// プレイヤーの更新
void UpdateHP();

// プレイヤーの描画
void DrawHP(int count);
