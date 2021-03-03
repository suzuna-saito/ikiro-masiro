#include "PlayerActor.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "AudioManager.h"

#include "PlayerActorStateBase.h"
#include "PlayerActorStateRun.h"
#include "PlayerActorStateIdle.h"
#include "PlayerActorStateJump.h"
#include "PlayerActorStateJumpLoop.h"
#include "PlayerActorStateJumpEnd.h"
#include "PlayerActorStateAttack1.h"
#include "PlayerActorStateAttack2.h"
#include "PlayerActorStateAttack3.h"
#include "PlayerActorStateHit.h"

const float cAnimationSpeed = 0.2f;

PlayerActor::PlayerActor()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mIsJump(false)
	, mOnGroundChk(true)
	, mWeaponMesh(nullptr)
{
	//メッシュのロード
	Mesh* mesh = RENDERER->GetMesh("Assets/valkiria.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// スケルトン
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/valkiria.gpskel"));
	
	// アニメーションの取得 & アニメーション配列にセット
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)]      = RENDERER->GetAnimation("Assets/valkiria_idle.gpanim"     , true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN)]       = RENDERER->GetAnimation("Assets/valkiria_Run.gpanim"      , true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPSTART)] = RENDERER->GetAnimation("Assets/valkiria_Jumpstart.gpanim", false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPLOOP)]  = RENDERER->GetAnimation("Assets/valkiria_jumploop.gpanim" , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPEND)]   = RENDERER->GetAnimation("Assets/valkiria_jumpend.gpanim"  , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_ATTACK1)]   = RENDERER->GetAnimation("Assets/valkiria_attack01.gpanim" , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_ATTACK2)]   = RENDERER->GetAnimation("Assets/valkiria_attack02.gpanim" , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_ATTACK3)]   = RENDERER->GetAnimation("Assets/valkiria_attack03.gpanim" , false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_HIT)]       = RENDERER->GetAnimation("Assets/valkiria_hit.gpanim"      , false);

	// アイドル状態アニメーションをセット
	mMeshComp->PlayAnimation( mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)], cAnimationSpeed);

	// アクターステートプールの初期化
	mStatePools.push_back(new PlayerActorStateIdle);       // mStatePool[PLAYER_STATE_IDLE]
	mStatePools.push_back(new PlayerActorStateRun);        // mStatepool[PLAYER_STATE_RUN]
	mStatePools.push_back(new PlayerActorStateJump);       // mStatepool[PLAYER_STATE_JUMPSTART];  
	mStatePools.push_back(new PlayerActorStateJumpLoop);   // mStatepool[PLAYER_STATE_JUMPLOOP];
	mStatePools.push_back(new PlayerActorStateJumpEnd);    // mStatepool[PLAYER_STATE_JUMPEND];
	mStatePools.push_back(new PlayerActorStateAttack1);    // mStatepool[PLAYER_STATE_ATTACK1];
	mStatePools.push_back(new PlayerActorStateAttack2);
	mStatePools.push_back(new PlayerActorStateAttack3);
	mStatePools.push_back(new PlayerActorStateHit); 

	// あたり判定セット
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	playerBox.mMin.x *= 1.2f;
	playerBox.mMin.y *= 1.2f;
	playerBox.mMax.x *= 1.2f;
	playerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(playerBox);

	// プレーヤーの足元を調べるボックスを作成　ボックス高1/4, ボックス上面が原点に来るようにする
	AABB groundBox;
	groundBox = playerBox;
	groundBox.mMin.x *= 0.8f;
	groundBox.mMin.y *= 0.8f;
	groundBox.mMax.x *= 0.8f;
	groundBox.mMax.y *= 0.8f;
	groundBox.mMin.z = -2.0f;  //ジャンプ時に引っかからない高さ
	groundBox.mMax.z *= 0.1f;
	mHitGroundBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitGroundBox->SetObjectBox(groundBox);

	// プレーヤーの頭上を調べるボックスを作成 ボックス底面が頭上に来るようにする
	AABB headBox;
	headBox = groundBox;
	headBox.mMin.z = playerBox.mMax.z;
	headBox.mMax.z = headBox.mMin.z + 2.0f;
	mHitHeadBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitHeadBox->SetObjectBox(headBox);
	
	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", mID, this);
	mJumpVec = Vector3(0, 0, 0);

	// 武器コンポーネント取付
    // 剣
	mesh = RENDERER->GetMesh("Assets/valkiria_Sword.gpmesh");
	mWeaponMesh = new AttachMeshComponent(this, mMeshComp, "index_01_r");
	mWeaponMesh->SetMesh(mesh);
	Vector3 rot, pos;
	rot = Vector3(-Math::PiOver2 * 0.5f, Math::Pi, 0);
	pos = Vector3(-55, 15, 110);
	mWeaponMesh->SetOffsetRotation(rot);
	mWeaponMesh->SetOffsetPosition(pos);

	// 武器ボックスの大きさ取得
	mWeaponAABB = mesh->GetCollisionBox();

	// 盾
	AttachMeshComponent* amc;
	amc = new AttachMeshComponent(this, mMeshComp, "lowerarm_l");
	amc->SetMesh(RENDERER->GetMesh("Assets/valkiria_Shield.gpmesh"));
	rot = Vector3(Math::PiOver2 * 0.5f, -Math::PiOver2 * 0.5f, -Math::PiOver2);
	pos = Vector3(60, 0, 120);
	amc->SetOffsetRotation(rot);
	amc->SetOffsetPosition(pos);

}

PlayerActor::~PlayerActor()
{
	// 登録ステートをすべて解放
	for (auto ps : mStatePools)
	{
		delete ps;
	}
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	// 前のシーンから床がなかったら
	if (!mIsJump && !mOnGroundChk)
	{
		mIsJump = true;
		mNextState = PlayerState::PLAYER_STATE_JUMPLOOP;
	}
	mOnGroundChk = false;

	// ステート外部からステート変更があったか？
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// ステート実行
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// ステート内部からステート変更あったか？
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	// 武器を振っているときの当たり判定の更新処理
	if (mAttackBox)
	{
		Matrix4 mat = mWeaponMesh->GetAttachTransMatrix();
		mAttackBox->SetForceTransForm(mat);
	}

}


// 背景AABBとのヒットめり込み解消 ( 当たった際にPhysicsWorldから呼ばれる ）
void PlayerActor::FixCollision(BoxCollider *hitPlayerBox, BoxCollider * hitBox)
{

	Vector3 fix;
	// ジャンプ終了か
	if (hitPlayerBox == mHitGroundBox && mJumpVec.z <= 0.0f)
	{
		// 地面判定用ボックスが地面と当たっていた(接地チェック）
		mOnGroundChk = true;

		// ジャンプ中なら解除
		if (mIsJump)
		{
			mOnGroundChk = true;
			mIsJump = false;
			mJumpVec = Vector3(0, 0, -1.0f);
			mPosition += mJumpVec;
				mNextState = PlayerState::PLAYER_STATE_JUMPEND;
			return;
		}
	}

	//壁とぶつかったとき
	AABB bgBox = hitBox->GetWorldBox();
	AABB playerBox = mHitBox->GetWorldBox();

	// めり込みを修正
	calcCollisionFixVec(playerBox, bgBox, fix);

	// 補正ベクトル分戻す
	mPosition += fix;

	// ジャンプアクション中に頭が壁に当たったとき
	if (hitPlayerBox == mHitHeadBox && mIsJump)
	{
		//ジャンプ上向きに行く方向を逆転させる
		mJumpVec = Vector3(0, 0, -1.0f);
		mPosition += mJumpVec; // もう頭上に当たらない様に離す
	}

	// 位置が変わったのでボックス再計算
	mHitBox->OnUpdateWorldTransform();

}

SkeletalMeshComponent* PlayerActor::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerActor::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void PlayerActor::SetAttackHitBox(float scale)
{
	mAttackBox = new BoxCollider(this, EnumPhysicsType::EnumPlayerAttack);

	AABB box = mWeaponAABB;
	box.mMin *= scale;
	box.mMax *= scale;

	mAttackBox->SetObjectBox(box);
}

void PlayerActor::RemoveAttackHitBox()
{
	if (mAttackBox)
	{
		delete mAttackBox;
		mAttackBox = nullptr;
	}
}

void PlayerActor::HitEnemy(class BoxCollider* playerBox, BoxCollider* enemyBox)
{
	//プレイヤーの攻撃が敵にヒットしたら、敵のほうを向く
	if (playerBox->GetType() == EnumPhysicsType::EnumPlayerAttack &&
		enemyBox->GetType()  == EnumPhysicsType::EnumEnemy)
	{
		Vector3 enemyPos = enemyBox->GetOwner()->GetPosition();
		Vector3 dir      = enemyPos - mPosition;
		Vector3 nowDir   = GetForward();
		dir.z = 0.0f;// 上向き方向をキャンセル

		//敵と接近しすぎて向きが取れない場合、90°以上の場合はそのまま
		const float epsiron = 0.0001f;
		float cos = Vector3::Dot(dir, nowDir);

		if (dir.LengthSq() < epsiron || cos < 0.0f)
		{
			dir = GetForward();
		}
		else
		{
			dir.Normalize();
		}
		RotateToNewForward(dir);
		RemoveAttackHitBox();
	}
}
