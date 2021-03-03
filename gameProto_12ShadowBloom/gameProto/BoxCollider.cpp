#include "BoxCollider.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "Actor.h"

// BoxCollider�R���X�g���N�^
BoxCollider::BoxCollider(Actor * owner, EnumPhysicsType physicsType, int updateOrder)
	: Component(owner, updateOrder)
	, mObjectBox(Vector3::Zero, Vector3::Zero)                    // �����l�͑傫���Ȃ��̃{�b�N�X��
	, mWorldBox(Vector3::Zero, Vector3::Zero)
	, mRotatable(true)
	, mIsTriggerType(false)
	, mHitTriggerFlag(false)
	, mPhisicsType(physicsType)
	, mIsIgnoreOwener(false)
{
	if (EnumPhysicsType::EnumBGTrigger == physicsType)
	{
		mIsTriggerType = true;
	}
	GAMEINSTANCE.GetPhysics()->AddBoxCollider(physicsType, this); // �{�b�N�X�R���C�_�[��PhysicsWirld�ɓo�^
	printf("Create : BoxCollider [%d] owner->( 0x%p )\n", GetID(), mOwner);
}

BoxCollider::~BoxCollider()
{
	printf("Remove : BoxCollider [%5d] owner->( 0x%p )\n", GetID(), mOwner);
	GAMEINSTANCE.GetPhysics()->RemoveBoxCollider(this);
}

void BoxCollider::OnUpdateWorldTransform()
{
	// �e�A�N�^�[����łȂ������ʒu���[�h�Ȃ�OnWorldTransform�𖳎�����
	if (mIsIgnoreOwener)
	{
		return;
	}
	// �I�u�W�F�N�g��Ԃ̃{�b�N�X�Ƀ��Z�b�g
	mWorldBox = mObjectBox;

	// �X�P�[�����O
	Vector3 scale = mOwner->GetScale();
	mWorldBox.mMin.x *= scale.x;
	mWorldBox.mMin.y *= scale.y;
	mWorldBox.mMin.z *= scale.z;

	mWorldBox.mMax.x *= scale.x;
	mWorldBox.mMax.y *= scale.y;
	mWorldBox.mMax.z *= scale.z;

	// ��]
	if (mRotatable)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	//���s�ړ�
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}

// �����I�Ƀ{�b�N�X�ɑ΂��ϊ��s��
void BoxCollider::SetForceTransForm(Matrix4 transform)
{
	mIsIgnoreOwener = true;
	// �I�u�W�F�N�g��Ԃ̃{�b�N�X�Ƀ��Z�b�g
	mWorldBox = mObjectBox;

	// �X�P�[�����O
	Vector3 scale = mOwner->GetScale();
	mWorldBox.mMin.x *= scale.x;
	mWorldBox.mMin.y *= scale.y;
	mWorldBox.mMin.z *= scale.z;

	mWorldBox.mMax.x *= scale.x;
	mWorldBox.mMax.y *= scale.y;
	mWorldBox.mMax.z *= scale.z;

	mWorldBox.mMin = Vector3::Transform(mWorldBox.mMin, transform);
	mWorldBox.mMax = Vector3::Transform(mWorldBox.mMax, transform);

}
