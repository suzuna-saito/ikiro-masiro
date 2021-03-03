// インクルードガード
// ヘッダーファイルの読み込みの重複を防ぐ
#pragma once

// Object構造体の前方宣言
struct Object;

//---------------------------------------
//	攻撃関連の変数、関数の「宣言」
//  (あくまでも実装はcppファイル)
//---------------------------------------

// 変数(使用宣言)-------------------------
extern Object attack[TAMA_MAX];		// 攻撃データ
extern int attackCount;

// 関数宣言（プロトタイプ宣言）------------
// こういう関数が使われますよという定義

// 攻撃の初期化
void InitializeAttack();

// 攻撃の更新
void UpdateAttack();

// 攻撃の描画
void DrawAttack();

// 攻撃の当たり判定
void CheckHitAttack(int _index);

// 攻撃の押し戻し
void PushBackAttack();

// 攻撃と敵が当たったか?
void HitAttackToEnemy(int _index);
void HitAttackToFlyEnemy(int _index);

//攻撃とブロックが当たったか
void HitAttackToBlock(int _index);
