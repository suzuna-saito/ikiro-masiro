#pragma once
#include "Math.h"

class BoneTransform
{
public:
	// For now, just make this data public
	Quaternion mRotation;                      // ��]
	Vector3 mTranslation;                      // �ړ�

	Matrix4 ToMatrix() const;                  // �s����o��

	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);  //�{�[�����
};
