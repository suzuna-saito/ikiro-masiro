// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Shader.h"
#include "Texture.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	: mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{

}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// ���_�V�F�[�_�[�A�s�N�Z���i�t���O�����g�j�V�F�[�_�[���R���p�C��
	if (!CompileShader(vertName,
		GL_VERTEX_SHADER,
		mVertexShader) ||
		!CompileShader(fragName,
			GL_FRAGMENT_SHADER,
			mFragShader))
	{
		return false;
	}

	//�����ŃV�F�[�_�[�v���O�������ł����̂ŁA ���_�V�F�[�_�[�E�t���O�����g�V�F�[�_�[�������N������
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// �v���O�����̃����N�������������H
	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}

void Shader::Unload()
{
	// �V�F�[�_�A�v���O������delete
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	// �V�F�[�_�[�v���O�������A�N�e�B�u�ɂ���
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// �V�F�[�_�[�ϐ�(uniform)�𖼑O�Ō�������
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// �s��f�[�^���V�F�[�_�[�ϐ��ɑ���
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// �s��z��f�[�^���V�F�[�_�[�ϐ��ɑ���
	glUniformMatrix4fv(loc, count, GL_TRUE, matrices->GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// �x�N�g���l���V�F�[�_�[�ϐ��ɑ���
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* name, const Vector2& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// �x�N�g���l���V�F�[�_�[�ϐ��ɑ���
	float v[2];
	v[0] = vector.x;
	v[1] = vector.y;
	glUniform2fv(loc, 1, v);
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// float�l���V�F�[�_�[�ϐ��ɑ���
	glUniform1f(loc, value);
}

void Shader::SetIntUniform(const char* name, int value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniform1i(loc, value);
}

bool Shader::CompileShader(const std::string& fileName,
	GLenum shaderType,
	GLuint& outShader)
{
	// �t�@�C�����J��
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// �e�L�X�g��ǂ݂���ŁA������ɕϊ�����
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// �V�F�[�_�[��shaderType�ɏ]���č쐬����
		outShader = glCreateShader(shaderType);
		// �\�[�X������̃R���p�C�������݂�
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			printf("�V�F�[�_�[�R���p�C���Ɏ��s %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		printf("�V�F�[�_�[�t�@�C����������܂���ł���: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// �R���p�C���̏�Ԃ�₢���킹��
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		printf("GLSL compile�Ɏ��s:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{

	GLint status;
	// �����N��Ԃ�₢���킹��
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		printf("GLSL Link ���:\n%s", buffer);
		return false;
	}

	return true;
}
