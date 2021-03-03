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
	// stateMap����state�N���X�S�������
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

	// ���݂̃X�e�[�g�̎��s
	EnemyStateEnum nextState = mNowState->Update(deltaTime);
	// �����X�e�[�g�������玟�̃X�e�[�g�ύX�w����������X�e�[�g�ύX������	
	if (mNowState->GetStateType() != nextState)
	{
		ChangeState(nextState);
	}
}

// �X�e�[�g�ύX����
void EnemyBehaviorComponent::ChangeState(EnemyStateEnum state)
{
	// ���݂̏�Ԃ��甲����
	if (mNowState)
	{
		mNowState->OnExit();
	}
	// �V������Ԃ�A�z�z�񂩂�T��
	auto iter = mStateMap.find(state);
	if (iter != mStateMap.end())
	{
		mNowState = iter->second;
		// �V������Ԃɓ���
		mNowState->OnEnter();
	}
	else
	{
		std::cout << "���o�^�X�e�[�g�ւ̈ڍs���s���܂��� : " << GetEnemyStateEnumName(mNowState->GetStateType()) << std::endl;
		mNowState = nullptr;
	}
}

// �X�e�[�g��o�^
void EnemyBehaviorComponent::RegisterState(EnemyState* state)
{
	// �X�e�[�g�^�C�v�ʂɓo�^
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
		std::cout << "���o�^�X�e�[�g : " << GetEnemyStateEnumName(state) << std::endl;
	}
}

// ���̃X�e�[�g�������Ă��邩
bool EnemyBehaviorComponent::HasState(EnemyStateEnum state)
{
	auto itr = mStateMap.find(state);
	return itr != mStateMap.end();
}
