#pragma once
#include "game.h"

// ����
struct Sphere
{
	Sphere(const Vector3& center, float radius);                      
	bool Contains(const Vector3& point) const;                        // ���̓��ɓ_���܂܂�邩�H

	Vector3 mCenter;                                                  // ���S�ʒu
	float mRadius;                                                    // ���a
};

// �����s�{�b�N�X(Axis-Aligned Bounding Box)
struct AABB
{
	AABB();
	AABB(const Vector3& min, const Vector3& max);                     // AABB�������p
	// ���f���ǂݍ��ݎ��̍ŏ��ő�̓_�����߂�̂Ɏg�p             
	void InitMinMax(const Vector3& point) { mMin = mMax = point; }    // 
	void UpdateMinMax(const Vector3& point);                          // �X�V����
	bool Contains(const Vector3& point) const;                        // �_���{�b�N�X���Ɋ܂܂�邩
	float MinDistSq(const Vector3& point) const;                      // �_�Ƃ̍ŏ����������߂�
	void Rotate(const Quaternion& q);                                 // ��]
	void SetArrowRotate(bool value) { mIsRotatable = value; }         // ��]�������邩�H

	Vector3 mMin;                                                     // �{�b�N�X�ŏ����W
	Vector3 mMax;                                                     // �{�b�N�X�ő���W 
	bool    mIsRotatable;                                             // ��]���邩���Ȃ����H
};


bool Intersect(const AABB& a, const AABB& b);                         // AABB ���m�̏Փ�
bool Intersect(const Sphere& s, const AABB& box);                     // ���̂�AABB�Ƃ̏Փ�

void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec); // AABB�̂߂荞�݂����ǂ��ʂ��v�Z
