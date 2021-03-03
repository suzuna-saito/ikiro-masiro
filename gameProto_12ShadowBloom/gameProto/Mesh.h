// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Renderer.h"
#include "Collision.h"
#include "rapidjson/rapidjson.h"


class Mesh
{
public:
	                   Mesh();
	                  ~Mesh();
	bool               Load(const std::string& fileName, class Renderer* renderer);   	// ���b�V���̃��[�h�E�A�����[�h
	void               Unload();                                                         	

	class VertexArray* GetVertexArray() { return mVertexArray; }         // ���b�V���̒��_�z��̎擾
	class Texture*     GetTexture(int index);                            // �w�肳�ꂽ�C���f�b�N�X����e�N�X�`���̎擾
	int                GetTextureID(TextureStage stage);

	const std::string& GetShaderName() const   { return mShaderName; }     // �V�F�[�_�[���̎擾
	const AABB&        GetCollisionBox() const { return mBox; }          // �R���W�����{�b�N�X���擾�i�I�u�W�F�N�g��ԁj
	float              GetRadius() const       { return mRadius; }             // �o�E���f�B���O�X�t�B�A�̔��a���擾
	float              GetSpecPower() const    { return mSpecPower; }
	float              GetLuminace() const     { return mLuminance; }

private:
	AABB                                  mBox;                          // AABB�R���W����
	std::vector<class Texture*>           mTextures;                     // ���b�V���̃e�N�X�`��
	class VertexArray*                    mVertexArray;                  // ���b�V���̒��_�z��

	std::string                           mShaderName;	                 // ���b�V���w��̃V�F�[�_�[
	float                                 mRadius;	                     // �o�E���f�B���O�X�t�B�A�̔��a
	float                                 mSpecPower;	                 // �\�ʂ̃X�y�L�����[�l
	float                                 mLuminance;                    // ���Ȕ������x(HDR)
	std::unordered_map<TextureStage, int> mStageDefTexture;              // �e�N�X�`���X�e�[�W�Ɋ��蓖�Ă�ꂽ�e�N�X�`��

};