#pragma once
#include "Actor.h"
#include "SkeletalMeshComponent.h"
#include "collision.h"
#include <string>
#include <unordered_map>

// 敵クラス状態enum 状態に変更があったらstateEnumNameも変更
enum class EnemyStateEnum : unsigned char
{
	Invalid,    // 無効なステート
	Spawn,      // 出現
	Idle,       // 待機状態
	Walk,       // 歩く
	Turn,       // ターン
	Run,        // 走る
	LookAround, // 見回す
	Attack1,    // 攻撃１
	Attack2,    // 攻撃２
	Attack3,    // 攻撃３
	GetDamage,  // ダメージ受けた
	Roar,       // 吠える
	Stun,       // 気絶
	Die,        // 死亡
	StateNum
};

// 敵トリガーボックス種類
enum class EnemyTriggerEnum : unsigned char
{
	ForwardBox,          // 正面
	LeftBox,             // 左側
	RightBox,            // 右
	BackBox,             // 後ろ
	HeadBox,             // 頭上
	GroundBox,           // 足元
	GroundForwardBox     // 足元前方
};

// ステート名を取得するヘルパー関数
const char* GetEnemyStateEnumName(EnemyStateEnum state);

// 敵ベースクラス
class EnemyActorBase : public Actor
{
public:
                   EnemyActorBase();
	virtual       ~EnemyActorBase() {};
	virtual void   UpdateActor(float deltaTime) = 0;
	bool           IsAnimationPlaying();
	float          PlayAnimation(EnemyStateEnum state, float rate = 1.0f);
	virtual void   OnCollision(class BoxCollider* hitThisBox, class BoxCollider* hitOtherBox) = 0;

	// セッター系
	void           SetVelocityVec(Vector3& v)   { mVelocityVec = v       ; }  // 速度ベクトルセット
	void           SetForwardVec(Vector3& v);								  // 前進ベクトルセット
	void           SetWalkSpeed(float speed)    { mWalkSpeed   = speed   ; }  // 歩きスピード
	void           SetTurnSpeed(float speed)    { mTurnSpeed   = speed   ; }  // 旋回スピード
	void           SetRunSpeed(float speed)     { mRunSpeed    = speed   ; }  // 走りスピード
	void           SetHitPoint(int hitpoint)    { mHitPoint    = hitpoint; }  // ヒットポイントセット
	void           SetIsOnGround(bool onGround) { mIsOnGround  = onGround; }  // 接地状態の変更

	void           SetTriggerBox(EnemyTriggerEnum trigerType, AABB& box);     // トリガーボックスのセット

	// ゲッター系
	const Vector3& GetVelocityVec() const { return mVelocityVec ; } // 速度ベクトルの取得
	const Vector3& GetForwardVec()  const { return mForwardVec  ; } // 速度ベクトルの取得
	float          GetWalkSpeed()   const { return mWalkSpeed   ; } // 歩きスピードの取得
	float          GetRunSpeed()    const { return mRunSpeed    ; } // 走りスピード取得
	float          GetTurnSpeed()   const { return mTurnSpeed   ; } // 旋回スピード取得
	int            GetHitPoint()    const { return mHitPoint    ; } // 体力の取得
	bool           GetIsOnGround()  const { return mIsOnGround  ; } // 接地状態の取得
	EnemyStateEnum GetEnemyBehaviorState() const { return mNowState; }

	bool           IsHitTrigger(EnemyTriggerEnum type);             // TriggerEnumがヒットしているか？
	bool           IsExistTriggerBox(EnemyTriggerEnum type);        // TriggerBoxが登録されているか？

protected:
	void           LoadAnimation(std::string& animationFileName, bool loop, EnemyStateEnum state);
	void           LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename);

	EnemyStateEnum mNowState    ; // 現在のステート

	Vector3        mVelocityVec ; // 速度ベクトル
	Vector3        mForwardVec  ; // 前進方向ベクトル  
	float          mWalkSpeed   ; // 歩きスピード
	float          mRunSpeed    ; // 走りスピード
	float          mTurnSpeed   ; // 旋回スピード
	int            mHitPoint    ; // 体力
	bool           mIsOnGround  ; // 地面にいる？

	SkeletalMeshComponent* mSkelMeshComponent; //メッシュデータ
	std::unordered_map<EnemyStateEnum, const class Animation*> mAnimations; // アニメーションセット
	std::unordered_map<EnemyTriggerEnum, class BoxCollider*>   mTrigerBoxs; // トリガーボックス
};