// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <cstdint>

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);                      // コンストラクタ updateOderが小さいと早く実行される
	virtual ~Component();                                                      // デストラクタ
	virtual void Update(float deltaTime);                                      // デルタタイムによるコンポーネントの更新 (オーバーライド可）
	virtual void ProcessInput() {}                                             // コンポーネントの入力処理（オーバーライド可）
	virtual void OnUpdateWorldTransform() { }                                  // ワールド変換が必要な時に呼ばれる関数（オーバーライド可）
	int GetID() { return mID; }
	class Actor* GetOwner() { return mOwner; }                                 // オーナーアクターの取得
	int GetUpdateOrder() const { return mUpdateOrder; }                        // update orderの取得
protected:
	class Actor* mOwner;                                                       // オーナーアクターへのポインタ
	int mUpdateOrder;                                                          // コンポーネントの更新順序
	int mID;                                                                   // コンポーネントの管理ID

	static int mGlobalID;                                                      // コンポーネントの管理IDの連番用
};
