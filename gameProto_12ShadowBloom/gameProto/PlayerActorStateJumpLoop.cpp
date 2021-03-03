#include "PlayerActorStateJumpLoop.h"
#include "PlayerActor.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "Input.h"

PlayerActorStateJumpLoop::PlayerActorStateJumpLoop()
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateJumpLoop\n");
}

PlayerActorStateJumpLoop::~PlayerActorStateJumpLoop()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateJumpLoop\n");
}

PlayerState PlayerActorStateJumpLoop::Update(PlayerActor* owner, float deltaTime)
{

	Move(owner, deltaTime);
	return PlayerState::PLAYER_STATE_JUMPLOOP;
}

void PlayerActorStateJumpLoop::Enter(PlayerActor* owner, float deltaTime)
{
	// アニメーションをジャンプ開始へ
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_JUMPLOOP));
}

void PlayerActorStateJumpLoop::Move(PlayerActor* owner, float deltaTime)
{
	//現在位置とジャンプ速度取得
	Vector3 position = owner->GetPosition();

	//重力加速度
	Vector3 jumpVec = owner->GetJumpVec();
	jumpVec.z -= 5.0f * deltaTime;
	owner->SetJumpVec(jumpVec);

	// 空中での方向キー移動入力
	const float speed = 200.0f;
	// カメラからみた前進方向を取得
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec = TargetPos - ViewPos;
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

	float nowSpeed = 0.0f;

	// キャラクター移動
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

	// ゲームパッドのLスティック入力
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rot);

	DirVec.x += stickL.x;
	DirVec.y += stickL.y;

	if (DirVec.LengthSq() > 0.5f)
	{
		// 方向キー入力
		charaForwardVec = DirVec;

		// 現在のスピードを保存
		nowSpeed = speed * deltaTime;
	}
	else
	{
		charaForwardVec = Vector3(0, 0, 0);
	}

	// 進行方向に移動
	position += nowSpeed * charaForwardVec * 0.8f + jumpVec;

	owner->SetPosition(position);
	owner->SetOnground(false);
	owner->SetComputeWorldTransform();

}
