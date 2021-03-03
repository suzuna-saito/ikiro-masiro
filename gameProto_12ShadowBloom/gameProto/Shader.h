// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	bool Load(const std::string& vertName, const std::string& fragName);          // ���_�V�F�[�_�[�A�t���O�����g�V�F�[�_�[�t�@�C���ǂݍ���
	void Unload();                                                                // �V�F�[�_�[�j��
	void SetActive();                                                             // ���̃V�F�[�_�[���A�N�e�B�u�ɂ���
	void SetMatrixUniform(const char* name, const Matrix4& matrix);               // �s��̃V�F�[�_�[�ϐ� name�ɒl���Z�b�g����
	void SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);  // �V�F�[�_�[�s��z�񖼂ɍs����Z�b�g����
	void SetVectorUniform(const char* name, const Vector3& vector);               // �x�N�g���l���V�F�[�_�[�ϐ�name�ɃZ�b�g����
	void SetVector2Uniform(const char* name, const Vector2& vector);               // �x�N�g���l���V�F�[�_�[�ϐ�name�ɃZ�b�g����
	void SetFloatUniform(const char* name, float value);                          // �ϐ��l���V�F�[�_�ϐ�name�ɃZ�b�g����
	void SetIntUniform(const char* name, int value);
private:
	bool CompileShader(const std::string& fileName,                               // �ǂݍ��܂ꂽ�V�F�[�_�[�̃R���p�C��
		GLenum shaderType,
		GLuint& outShader);

	bool IsCompiled(GLuint shader);                                               // �R���p�C�������������H
	bool IsValidProgram();                                                        // vertex,fragment�Ƃ��Ƀ����N�������Ă邩�H
private:
	// Store the shader object IDs
	GLuint mVertexShader;                                                         // ���_�V�F�[�_�[ID
	GLuint mFragShader;                                                           // �t���O�����g�V�F�[�_�[ID
	GLuint mShaderProgram;                                                        // �V�F�[�_�[�v���O�����i���_�{�t���O�����g�jID 
};
