#include "SwordEffectActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"


Quaternion rotateFromVector(Vector3 forward)
{
	// X���x�N�g��(1,0,0)��forward�̊Ԃ̊p�x�����߂�
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	
	// �������������ꍇ
	if (dot > 0.9999f)
	{
		 return Quaternion::Identity;
	}
	// ������������ꍇ
	else if (dot < -0.9999f)
	{
		return Quaternion(Vector3::UnitZ, Math::Pi);
	}
	else
	{
		// ���x�N�g����forward�Ƃ̊O�ς����]�������Ƃ߁A��]������
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		return Quaternion(axis, angle);
	}

}

SwordEffectActor::SwordEffectActor(Actor* effectOrignActor)
	: mEffectOrign(effectOrignActor)
	, mEffectTime(1.0f)
{
	MeshComponent* m = new MeshComponent(this);
	m->SetMesh(RENDERER->GetMesh("assets/swordeffect.gpmesh"));
}

SwordEffectActor::~SwordEffectActor()
{
}

void SwordEffectActor::UpdateActor(float deltaTime)
{
	mEffectTime -= deltaTime;
	if (mEffectTime < 0.0f)
	{
		mState = State::EDead;
	}


	//���[���h�s��Z�o
	Matrix4 localRotate, ownerRotate, localPosMat, ownerPosMat;

	ownerRotate = Matrix4::CreateFromQuaternion(rotateFromVector(mEffectOrign->GetForward()));
	localPosMat = Matrix4::CreateTranslation(mLocalPos);
	ownerPosMat = Matrix4::CreateTranslation(mEffectOrign->GetPosition());

	mWorldTransform = mLocalRotation * localPosMat * ownerRotate * ownerPosMat;
}

void SwordEffectActor::SetLocalRotation(const Vector3& angleRad)
{
	mLocalRotation = Matrix4::CreateRotationX(angleRad.x) *
		             Matrix4::CreateRotationY(angleRad.y) *
		             Matrix4::CreateRotationZ(angleRad.z);
}
