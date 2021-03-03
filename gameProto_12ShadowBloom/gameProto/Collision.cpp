#include "Collision.h"
#include <algorithm>
#include <array>

Sphere::Sphere(const Vector3 & center, float radius)
	:mCenter(center)
	, mRadius(radius)
{
}

bool Sphere::Contains(const Vector3 & point) const
{
	//���̒��S�Ɠ_�Ƃ̋����̓����v�Z�������̂Ɣ��a��2��Ɣ�r
	float distSq = (mCenter - point).LengthSq();
	return distSq <= mRadius * mRadius;
}

AABB::AABB()
{
	mMin = Vector3(0, 0, 0);
	mMax = Vector3(0, 0, 0);
}

AABB::AABB(const Vector3 & min, const Vector3 & max)
	:mMin(min)
	,mMax(max)
{
}

void AABB::UpdateMinMax(const Vector3 & point)
{
	// x,y,z���ꂼ��̍ő�ŏ������߂�
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);

	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);
}

bool AABB::Contains(const Vector3 & point) const
{
	bool outside = point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;
	// �������true���Ȃ���΁Abox�̓����ɓ_�����݂���
	return !outside;
}

float AABB::MinDistSq(const Vector3 & point) const
{
	// ���ꂼ��̎��ł̍����Ƃ�
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - mMax.z);
	// 3������Ԃł̋����̓��̌������
	return dx * dx + dy * dy + dz * dz;
}

void AABB::Rotate(const Quaternion & q)
{
	// �{�b�N�X��8�̒��_�̔z����
	std::array<Vector3, 8> points;
	// �ŏ��l�͏�ɃR�[�i�[�ł���
	points[0] = mMin;
	// 2�̍ŏ��l��1�̍ő�l�̕��בւ�
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);
	// 2�̍ő�l��1�̍ŏ��l�̕��בւ�
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);
	// �ő�l�͏�ɃR�[�i�[�ł���
	points[7] = Vector3(mMax);

	// �ŏ��̓_����]
	Vector3 p = Vector3::Transform(points[0], q);
	// �ŏ��l�ƍő�l�����߂邽�߂ɍŏ��̓_�����ă��Z�b�g
	mMin = p;
	mMax = p;
	// ��]�ɂ���čő�l�ƍŏ��l�����߂Ȃ����B
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

// AABB�ǂ����̏Փ�
bool Intersect(const AABB & a, const AABB & b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	// ���true���Ȃ���΁A��͏Փ˂��Ă���
	return !no;
}
//����AABB�̏Փ�
bool Intersect(const Sphere& s, const AABB & box)
{
	float distSq = box.MinDistSq(s.mCenter);
	return distSq <= (s.mRadius * s.mRadius);
}

////////////////////////////////////////////////////////////////////
// �Փ˂������Ƃ��m�肵���Ƃ��A�߂荞�݂�߂��֐�
// in    movableBox �ړ����� (ex �v���[���[)
// in    fixedBox   �ړ����Ȃ����́iex �u���b�N�j
// inout calcFixVec �ړ����̂̕␳�����x�N�g��
////////////////////////////////////////////////////////////////////
void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec)
{
	calcFixVec = Vector3(0, 0, 0);
	float dx1 = fixedBox.mMin.x - movableBox.mMax.x;
	float dx2 = fixedBox.mMax.x - movableBox.mMin.x;
	float dy1 = fixedBox.mMin.y - movableBox.mMax.y;
	float dy2 = fixedBox.mMax.y - movableBox.mMin.y;
	float dz1 = fixedBox.mMin.z - movableBox.mMax.z;
	float dz2 = fixedBox.mMax.z - movableBox.mMin.z;

	// dx, dy, dz �ɂ� ���ꂼ��1,2�̂�����Βl�������������Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		calcFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		calcFixVec.y = dy;
	}
	else
	{
		calcFixVec.z = dz;
	}
}

