// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor();
	virtual ~Actor();

	void              Update(float deltaTime);                                                     // Update関数　Gameからコールされる（オーバーライド不可）
	void              UpdateComponents(float deltaTime);                                           // アタッチされてる全てのコンポーネントたちの更新（オーバーライド不可）
	virtual void      UpdateActor(float deltaTime);                                                // アクター専用のアップデート　（オーバーライド可能）
	void              ProcessInput();                                                              // ゲームから呼ばれる入力関連（オーバーライド不可）

	// Getters/setters　ゲッター・セッター
	const Vector3&    GetPosition() const { return mPosition; }                                                            // 位置のゲット
	void              SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }                // 位置のセット
	const Vector3&    GetScale() const { return mScale; }                                                                  // スケーリングのゲット
	void              SetScale(float scale);                                                       // スケーリングのセット
	void              SetScale(const Vector3& scale);
	const Quaternion& GetRotation() const { return mRotation; }                                                            // 回転（クォータニオン）のゲット
	void              SetRotation(const Quaternion& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }  // 回転（クォータニオン）のセット
	void              SetRotation(const Vector3& rotation);

	void              ComputeWorldTransform();                                                     // ワールド変換行列の計算
	void              SetComputeWorldTransform() { mRecomputeWorldTransform = true; }              // 移動など変換行列の再計算が必要な時
	const Matrix4&    GetWorldTransform() const { return mWorldTransform; }                        // ワールド変換行列のゲット
	Vector3           GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); } // 前進ベクトルのゲット
	Vector3           GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }   // 右方向ベクトルのゲット
	State             GetState() const { return mState; }                                          // アクターの状態ゲット
	float             GetSpeed() const { return mSpeed; }                                          // アクターの現在のスピード
	void              SetSpeed(float speed) { mSpeed = speed; }
	void              RotateToNewForward(const Vector3& forward);                                  // 前進ベクトルのセット
	void              SetState(State state) { mState = state; }                                    // アクターの状態セット
	int               GetID() { return mID; };                                                     // アクターの管理IDのゲット
	void              AddComponent(class Component* component);                                    // コンポーネントの追加 
	void              RemoveComponent(class Component* component);                                 // コンポーネントの削除 

protected:
	State             mState;                                                                      // アクターの状態
	Matrix4           mWorldTransform;                                                             // ワールド変換行列
	Vector3           mPosition;                                                                   // 位置
	Quaternion        mRotation;                                                                   // 回転
	Vector3           mRotationXYZ;
	Vector3           mScale;
	float             mSpeed;                                                                      // 現在のスピード
	bool              mRecomputeWorldTransform;                                                    // ワールド変換の再計算が必要か？
	std::vector<class Component*> mComponents;                                                     // コンポーネント配列
	class Game*       mGame;                                                                       // ゲームシステムクラスへのポインタ
	int               mID;                                                                         // 管理ID
	static int        mGlobalActorNo;                                                              // 管理ID用の静的カウンタ
};
