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
	// �R���g���[�����͂��ꂽ��
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	//�����L�[�����͂��ꂽ��
	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		isContollerInputOff;

	bool IsAttack = INPUT_INSTANCE.IsKeyPushdown(KEY_A);
	bool IsJump = INPUT_INSTANCE.IsKeyPushdown(KEY_B);

	// ������̃{�^����������Ă��Ȃ�
	if (IsIdle && !IsJump && !IsAttack)
	{
		return PlayerState::PLAYER_STATE_IDLE;
	}
	// �U���{�^���������ꂽ���H
	if (IsAttack)
	{
		return PlayerState::PLAYER_STATE_ATTACK1;
	}

	// �W�����v�{�^���������ꂽ���H
	if (IsJump)
	{
		return PlayerState::PLAYER_STATE_JUMPSTART;
	}

	// �ړ�����
	MoveCalc(owner, deltaTime);

	return PlayerState::PLAYER_STATE_RUN;
}

// RUN��Ԃւ̐؂�ւ�����
void PlayerActorStateRun::Enter(PlayerActor* owner, float deltaTime)
{
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_RUN),0.7f);
}

// �ړ�����
void PlayerActorStateRun::MoveCalc(PlayerActor* owner, float deltaTime)
{
	//�L��������
	const float speed = 350.0f;
	float charaSpeed = owner->GetSpeed(); //  �L�����̌��݂̃X�s�[�h

	// �J��������݂��O�i�������擾
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos   = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec= TargetPos - ViewPos;
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

	// �L�����N�^�[�ړ��̓��̓L�[�擾
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

	// �Q�[���p�b�h
	Vector2 stickL = INPUT_INSTANCE.GetLStick();

	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rot);

	DirVec.x += stickL.x;
	DirVec.y += stickL.y;

	// ���̓L�[�̑��a
	if (DirVec.LengthSq() > 0.5f)
	{
		// �����L�[����
		charaForwardVec = DirVec;

		// �i�s�����Ɍ����ĉ�]
		charaForwardVec.Normalize();
		owner->RotateToNewForward(charaForwardVec);

		// ���݂̃X�s�[�h��ۑ�
		charaSpeed = speed * deltaTime;
	}

	// �ړ�����
	Vector3 position = owner->GetPosition();
	position += charaSpeed * charaForwardVec;

	// �L�����̈ʒu�E�X�s�[�h�E�ϊ��s��̍Čv�Z�̕K�v���Z�b�g
	owner->SetPosition(position);
	owner->SetSpeed(charaSpeed);
	owner->SetComputeWorldTransform();
}
