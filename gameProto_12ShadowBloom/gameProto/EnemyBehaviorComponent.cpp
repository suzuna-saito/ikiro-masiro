#include "EnemyBehaviorComponent.h"
#include "EnemyState.h"
#include <iostream>

EnemyBehaviorComponent::EnemyBehaviorComponent(EnemyActorBase* owner)
	: Component(owner)
	, mNowState(nullptr)
{
	printf("Create : EnemyBehaviorComponent [%d]\n", mID);
}

EnemyBehaviorComponent::~EnemyBehaviorComponent()
{
	// stateMap内のstateクラス全解放処理
	for (auto i = mStateMap.begin(); i != mStateMap.end(); ++i)
	{
		delete i->second;
	}
	printf("remove : EnemyBehaviorComponent [%d]\n", mID);
}

void EnemyBehaviorComponent::Update(float deltaTime)
{
	if (!mNowState)
	{
		return;
	}

	// 現在のステートの実行
	EnemyStateEnum nextState = mNowState->Update(deltaTime);
	// もしステート内部から次のステート変更指示が来たらステート変更処理へ	
	if (mNowState->GetStateType() != nextState)
	{
		ChangeState(nextState);
	}
}

// ステート変更処理
void EnemyBehaviorComponent::ChangeState(EnemyStateEnum state)
{
	// 現在の状態から抜ける
	if (mNowState)
	{
		mNowState->OnExit();
	}
	// 新しい状態を連想配列から探す
	auto iter = mStateMap.find(state);
	if (iter != mStateMap.end())
	{
		mNowState = iter->second;
		// 新しい状態に入る
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "未登録ステートへの移行が行われました : " << GetEnemyStateEnumName(mNowState->GetStateType()) << std::endl;
		mNowState = nullptr;
	}
}

// ステートを登録
void EnemyBehaviorComponent::RegisterState(EnemyState* state)
{
	// ステートタイプ別に登録
	mStateMap.emplace(state->GetStateType(), state);
}

void EnemyBehaviorComponent::SetFirstState(EnemyStateEnum state)
{
	if (HasState(state))
	{
		mNowState = mStateMap[state];
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "未登録ステート : " << GetEnemyStateEnumName(state) << std::endl;
	}
}

// そのステートを持っているか
bool EnemyBehaviorComponent::HasState(EnemyStateEnum state)
{
	auto itr = mStateMap.find(state);
	return itr != mStateMap.end();
}
