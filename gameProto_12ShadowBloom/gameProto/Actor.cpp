// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <typeinfo>

int Actor::mGlobalActorNo = 0;

// Actorコンストラクタ  
Actor::Actor()
	: mState(EActive)
	, mGame(nullptr)
	, mPosition(Vector3::Zero)
	, mRotation(Quaternion::Identity)
	, mRecomputeWorldTransform(true)
	, mID(mGlobalActorNo)
	, mSpeed(0.0f)
{
	mScale = Vector3(1, 1, 1);
	//ゲームシステム本体に アクターを追加
	GAMEINSTANCE.AddActor(this);
	mGlobalActorNo++;
}

Actor::~Actor()
{
	// アクターが持っているコンポーネントの削除
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
	// ゲームシステム本体にこのアクターの削除を依頼
	GAMEINSTANCE.RemoveActor(this);
}

// Update関数　Gameからコールされる
// 引数 in : deltaTime  1フレーム分の経過時間
void Actor::Update(float deltaTime)
{
	// アクターが生きているときのみ処理
	if (mState == EActive)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

// 全てのコンポーネントの更新処理
// 引数 in : deltaTime  1フレーム分の経過時間
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

// このアクター独自の更新処理 （必要ならオーバーライド）
void Actor::UpdateActor(float deltaTime)
{
}

// このアクターが持っているコンポーネントの入力処理
void Actor::ProcessInput()
{
	if (mState == EActive)
	{
		// 入力処理を受け取るコンポーネントを優先して実行
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
	}
}

void Actor::SetScale(float scale)
{
	mScale = Vector3(scale, scale, scale);
	mRecomputeWorldTransform = true;
}

void Actor::SetScale(const Vector3& scale)
{
	mScale = scale;
	mRecomputeWorldTransform = true;
}

// forwardベクトルの向きに回転する
// in forward : 向かせたい前方方向ベクトル
void Actor::RotateToNewForward(const Vector3& forward)
{
	// X軸ベクトル(1,0,0)とforwardの間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとforwardとの外積から回転軸をもとめ、回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

// x,y,z回転値から回転をセット
void Actor::SetRotation(const Vector3& rotation)
{
	Matrix4 mat;
	mat =  Matrix4::CreateRotationX(rotation.x);
	mat *= Matrix4::CreateRotationY(rotation.y);
	mat *= Matrix4::CreateRotationZ(rotation.z);

	mRotation = Quaternion::QuaeternionFromMatrix(mat);
	mRecomputeWorldTransform = true;
}

// ワールド変換行列を計算
// 引数 なし
void Actor::ComputeWorldTransform()
{
	//ワールド変換の再計算が必要なら実行
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		// スケーリング→回転→平行移動となるように変換行列を作成
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// アクターが持っている全コンポーネントの変換を促す
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

// コンポーネントの追加
// 引数　in : component 追加コンポーネントへのポインタ
void Actor::AddComponent(Component* component)
{
	// コンポーネントをソートして追加
	// 自分のオーダー番号よりも大きい挿入点を見つける
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// 要素を見つけたポイントの手前に挿入する
	mComponents.insert(iter, component);
}

// コンポーネントの削除
// 引数　in : component  削除コンポーネントへのポインタ 
void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
