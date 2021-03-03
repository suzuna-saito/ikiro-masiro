#include "PlayerActorStateRun.h"
#include "Input.h"
#include "SkeletalMeshComponent.h"

PlayerActorStateRun::PlayerActorStateRun()
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateRun\n");
}

PlayerActorStateRun::~PlayerActorStateRun()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateRun\n");
}

PlayerState PlayerActorStateRun::Update(PlayerActor* owner, float deltaTime)
{
	// コントローラ入力されたか
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	//方向キーが入力されたか
	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		isContollerInputOff;

	bool IsAttack = INPUT_INSTANCE.IsKeyPushdown(KEY_A);
	bool IsJump = INPUT_INSTANCE.IsKeyPushdown(KEY_B);

	// いずれのボタンも押されていない
	if (IsIdle && !IsJump && !IsAttack)
	{
		return PlayerState::PLAYER_STATE_IDLE;
	}
	// 攻撃ボタンが押されたか？
	if (IsAttack)
	{
		return PlayerState::PLAYER_STATE_ATTACK1;
	}

	// ジャンプボタンが押されたか？
	if (IsJump)
	{
		return PlayerState::PLAYER_STATE_JUMPSTART;
	}

	// 移動処理
	MoveCalc(owner, deltaTime);

	return PlayerState::PLAYER_STATE_RUN;
}

// RUN状態への切り替え処理
void PlayerActorStateRun::Enter(PlayerActor* owner, float deltaTime)
{
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_RUN),0.7f);
}

// 移動処理
void PlayerActorStateRun::MoveCalc(PlayerActor* owner, float deltaTime)
{
	//キャラ入力
	const float speed = 350.0f;
	float charaSpeed = owner->GetSpeed(); //  キャラの現在のスピード

	// カメラからみた前進方向を取得
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos   = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec= TargetPos - ViewPos;
	forwardVec.z = 0.0; // 高さ方向を無視

	// カメラ前方ベクトルと右方向ベクトル算出
	forwardVec = Vector3::Normalize(forwardVec);
	Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// 右方向ベクトルからカメラ回転角を算出
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));

	// 右回転か左回転か？
	tmpVec = Vector3::Cross(Vector3::UnitX, rightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	Vector3 charaForwardVec = owner->GetForward(); // キャラの前進ベクトル

	// キャラクター移動の入力キー取得
	Vector3 DirVec(0.0f, 0.0f, 0.0f);
	if (INPUT_INSTANCE.IsKeyPressed(KEY_UP))
	{
		DirVec += forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_DOWN))
	{
		DirVec -= forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_RIGHT))
	{
		DirVec += rightVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_LEFT))
	{
		DirVec -= rightVec;
	}

	// ゲームパッド
	Vector2 stickL = INPUT_INSTANCE.GetLStick();

	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rot);

	DirVec.x += stickL.x;
	DirVec.y += stickL.y;

	// 入力キーの総和
	if (DirVec.LengthSq() > 0.5f)
	{
		// 方向キー入力
		charaForwardVec = DirVec;

		// 進行方向に向けて回転
		charaForwardVec.Normalize();
		owner->RotateToNewForward(charaForwardVec);

		// 現在のスピードを保存
		charaSpeed = speed * deltaTime;
	}

	// 移動処理
	Vector3 position = owner->GetPosition();
	position += charaSpeed * charaForwardVec;

	// キャラの位置・スピード・変換行列の再計算の必要をセット
	owner->SetPosition(position);
	owner->SetSpeed(charaSpeed);
	owner->SetComputeWorldTransform();
}
