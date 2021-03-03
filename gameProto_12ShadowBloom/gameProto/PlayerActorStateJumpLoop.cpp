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
	// �A�j���[�V�������W�����v�J�n��
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_JUMPLOOP));
}

void PlayerActorStateJumpLoop::Move(PlayerActor* owner, float deltaTime)
{
	//���݈ʒu�ƃW�����v���x�擾
	Vector3 position = owner->GetPosition();

	//�d�͉����x
	Vector3 jumpVec = owner->GetJumpVec();
	jumpVec.z -= 5.0f * deltaTime;
	owner->SetJumpVec(jumpVec);

	// �󒆂ł̕����L�[�ړ�����
	const float speed = 200.0f;
	// �J��������݂��O�i�������擾
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec = TargetPos - ViewPos;
	forwardVec.z = 0.0; // ���������𖳎�

	// �J�����O���x�N�g���ƉE�����x�N�g���Z�o
	forwardVec = Vector3::Normalize(forwardVec);
	Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// �E�����x�N�g������J������]�p���Z�o
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));

	// �E��]������]���H
	tmpVec = Vector3::Cross(Vector3::UnitX, rightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;
	Vector3 charaForwardVec = owner->GetForward(); // �L�����̑O�i�x�N�g��

	float nowSpeed = 0.0f;

	// �L�����N�^�[�ړ�
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

	// �Q�[���p�b�h��L�X�e�B�b�N����
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rot);

	DirVec.x += stickL.x;
	DirVec.y += stickL.y;

	if (DirVec.LengthSq() > 0.5f)
	{
		// �����L�[����
		charaForwardVec = DirVec;

		// ���݂̃X�s�[�h��ۑ�
		nowSpeed = speed * deltaTime;
	}
	else
	{
		charaForwardVec = Vector3(0, 0, 0);
	}

	// �i�s�����Ɉړ�
	position += nowSpeed * charaForwardVec * 0.8f + jumpVec;

	owner->SetPosition(position);
	owner->SetOnground(false);
	owner->SetComputeWorldTransform();

}
