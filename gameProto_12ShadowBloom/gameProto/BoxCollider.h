#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"

enum class EnumPhysicsType;

class BoxCollider : public Component
{
public:
	BoxCollider(class Actor* owner, EnumPhysicsType physicsType,int updateOrder = 100);
	~BoxCollider();
	void OnUpdateWorldTransform() override;                       // ���[���h�ϊ���
	void SetObjectBox(const AABB& box) { mObjectBox = box; }      // �����蔻��p���E�{�b�N�X���Z�b�g
	const AABB& GetWorldBox() const    { return mWorldBox; }      // ���[���h��ԏ�ł̋��E�{�b�N�X���擾
	void  SetArrowRotate(bool value)   { mRotatable = value; }    // ��]�������邩�H
	EnumPhysicsType GetType()          { return mPhisicsType; }   // �����̓����蔻�葮��

	bool  IsTrigerHit()                { return mHitTriggerFlag; }// �g���K�[���ɉ����ƃq�b�g�������H
	void  SetHitTriggerFlag(bool hit)  { mHitTriggerFlag = hit; } // �g���K�[���Ƀq�b�g���������Z�b�g
	void  SetForceTransForm(Matrix4 transform);                   // �����蔻��{�b�N�X�̈ړ���

private:
	AABB            mObjectBox;                                   // �I�u�W�F�N�g��ԁi�ϊ��O�j�̃{�b�N�X
	AABB            mWorldBox;                                    // ���[���h��Ԃɒu�������̃{�b�N�X
	bool            mRotatable;                                   // ��]�������邩�H
	bool            mHitTriggerFlag;                              // �g���K�[���ɉ����ƃq�b�g������
	bool            mIsTriggerType;                               // ����̓g���K�[�^�C�v��
	bool            mIsIgnoreOwener;                              // �I�[�i�[�̈ړ��𖳎����邩
	EnumPhysicsType mPhisicsType;

	friend class PhysicsWorld;                                    
};