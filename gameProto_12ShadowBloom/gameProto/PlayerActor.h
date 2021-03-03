#pragma once
#include "Actor.h"
#include "Collision.h"

enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,   // 待機
	PLAYER_STATE_RUN,        // 走る
	PLAYER_STATE_JUMPSTART,  // ジャンプスタート
	PLAYER_STATE_JUMPLOOP,   // ジャンプループ    
	PLAYER_STATE_JUMPEND,    // ジャンプ終了
	PLAYER_STATE_ATTACK1,    // 攻撃1
	PLAYER_STATE_ATTACK2,    // 攻撃2
	PLAYER_STATE_ATTACK3,    // 攻撃3
	PLAYER_STATE_HIT,

	PLAYER_STATE_NUM,   // 総アニメーション数
};

class PlayerActor : public Actor
{
public:
	PlayerActor();
	~PlayerActor();

	void                         UpdateActor(float deltaTime) override;
	void                         FixCollision(class BoxCollider *hitPlayerBox, class BoxCollider *hitBox);
	class SkeletalMeshComponent* GetSkeletalMeshComp();
	const class Animation*       GetAnim(PlayerState state);
	bool                         GetIsJump()const { return mIsJump; }
	bool                         GetOnground() const { return mOnGroundChk; }
	Vector3&                     GetJumpVec() { return mJumpVec; }

	void                         SetOnground(bool onGround) { mOnGroundChk = onGround; }
	void                         SetIsJump(bool jump) { mIsJump = jump; }
	void                         SetJumpVec(Vector3& vec) { mJumpVec = vec; }
	void                         SetAttackHitBox(float scale = 1.0f);
	void                         RemoveAttackHitBox();
	void                         HitEnemy(class BoxCollider* playerBox, class BoxCollider* enemyBox);

private:
	class SkeletalMeshComponent*             mMeshComp;
	std::vector<const class Animation*>      mAnimTypes;
	class BoxCollider*                       mHitBox;
	class BoxCollider*                       mHitGroundBox;
	class BoxCollider*                       mHitHeadBox;
	class BoxCollider*                       mAttackBox;       // プレイヤーの攻撃当たり判定ボックス                                            
	class AttachMeshComponent*               mWeaponMesh;      // 武器メッシュ

	PlayerState                              mNowState;        // 現在のステート
	PlayerState                              mNextState;       // 次のステート
	std::vector<class PlayerActorStateBase*> mStatePools;      // ステートクラスプール

	bool                                     mIsJump;
	bool                                     mOnGroundChk;
	Vector3                                  mJumpVec;
	Vector3                                  mVelocityVec;
	AABB                                     mWeaponAABB;
};