#pragma once
#include "MeshComponent.h"
#include "Math.h"
#include <cstddef>


// スケルタルメッシュの関節に取り付け可能なメッシュ
class AttachMeshComponent : public MeshComponent
{
public:
	AttachMeshComponent(class Actor* owner, class SkeletalMeshComponent* skMesh, const char* AttachBoneName);
	~AttachMeshComponent();
	void SetOffsetPosition(class Vector3& offset);
	void SetOffsetRotation(class Vector3& rotation);

	void Draw(class Shader* shader)override;
	const Vector3& GetAttachPosisiton()const { return mComputeAttachPos; }
	const Matrix4& GetAttachTransMatrix()const { return mComputeTransMatrix; }
protected:
	Matrix4                      mOffsetPos;
	Matrix4                      mOffsetRotation;
	Matrix4                      mComputeTransMatrix;
	int                          mAttachBoneIndex;
	Vector3                      mComputeAttachPos;
	class SkeletalMeshComponent* mAttachSkeletalMesh;

};