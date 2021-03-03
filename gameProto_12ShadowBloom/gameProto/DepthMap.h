#pragma once
#include "Game.h"

class DepthMap
{
public:
	DepthMap();
	~DepthMap();
	void          DepthRenderingBegin();
	void          DepthRenderignEnd();
	void          CreateShadowMap(unsigned int shadowMapSize);
	void          CalcLightSpaceMatrix(const Vector3& centerWorldPos,
	                                   const Vector3& lightDir,
                                       const Vector3& upVec,
	                                   float lightDistance);

	Matrix4&      GetLightSpaceMatrix() { return mLightSpaceMatrix; }
	Vector3&      GetLightDir() { return mLightDir; }
	Vector3&      GetLightOriginPos() { return mLightOrigin; }
	class Shader* GetDepthMapShader() { return mDepthShader; }
	unsigned int  GetDepthTexID() { return mDepthMap; }

private:
	class Shader* mDepthShader; //�f�v�X�}�b�v�p�V�F�[�_�[

	unsigned int  mDepthMapFBO; // �f�v�X�}�b�v�t���[���o�b�t�@�I�u�W�F�N�g
	unsigned int  mDepthMap; // �f�v�X�e�N�X�`��
	unsigned int  mDepthMapSize; // �f�v�X�}�b�v�T�C�Y

	Vector3       mLightOrigin; // �f�v�X�}�b�v�����_�����O�̎B�e���_
	Vector3       mLightDir; // �����x�N�g������
	Matrix4       mLightSpaceMatrix; // ���C�g��ԍs��
};