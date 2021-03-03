#pragma once
#include "EnemyActorBase.h"
#include "Component.h"
#include <unordered_map>

class EnemyBehaviorComponent : public Component
{
public:
	EnemyBehaviorComponent(class EnemyActorBase* owner);
	~EnemyBehaviorComponent();

	void Update(float deltaTime) override;        // ステート処理
	void ChangeState(EnemyStateEnum state);       // ステート変更
	void RegisterState(class EnemyState* state);  // ステートの登録
	void SetFirstState(EnemyStateEnum state);
	bool HasState(EnemyStateEnum state);          // そのステートを持っているか？
	class EnemyActorBase* GetOwnerActor() { return static_cast<EnemyActorBase*>(mOwner); }

protected:
	std::unordered_map<EnemyStateEnum, class EnemyState*> mStateMap;
	class EnemyState* mNowState;
};