#pragma once
#include "EnemyActorBase.h"
#include "Component.h"
#include <unordered_map>

class EnemyBehaviorComponent : public Component
{
public:
	EnemyBehaviorComponent(class EnemyActorBase* owner);
	~EnemyBehaviorComponent();

	void Update(float deltaTime) override;        // �X�e�[�g����
	void ChangeState(EnemyStateEnum state);       // �X�e�[�g�ύX
	void RegisterState(class EnemyState* state);  // �X�e�[�g�̓o�^
	void SetFirstState(EnemyStateEnum state);
	bool HasState(EnemyStateEnum state);          // ���̃X�e�[�g�������Ă��邩�H
	class EnemyActorBase* GetOwnerActor() { return static_cast<EnemyActorBase*>(mOwner); }

protected:
	std::unordered_map<EnemyStateEnum, class EnemyState*> mStateMap;
	class EnemyState* mNowState;
};