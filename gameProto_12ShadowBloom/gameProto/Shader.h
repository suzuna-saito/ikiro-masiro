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
	bool Load(const std::string& vertName, const std::string& fragName);          // 頂点シェーダー、フラグメントシェーダーファイル読み込み
	void Unload();                                                                // シェーダー破棄
	void SetActive();                                                             // このシェーダーをアクティブにする
	void SetMatrixUniform(const char* name, const Matrix4& matrix);               // 行列のシェーダー変数 nameに値をセットする
	void SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);  // シェーダー行列配列名に行列をセットする
	void SetVectorUniform(const char* name, const Vector3& vector);               // ベクトル値をシェーダー変数nameにセットする
	void SetVector2Uniform(const char* name, const Vector2& vector);               // ベクトル値をシェーダー変数nameにセットする
	void SetFloatUniform(const char* name, float value);                          // 変数値をシェーダ変数nameにセットする
	void SetIntUniform(const char* name, int value);
private:
	bool CompileShader(const std::string& fileName,                               // 読み込まれたシェーダーのコンパイル
		GLenum shaderType,
		GLuint& outShader);

	bool IsCompiled(GLuint shader);                                               // コンパイル成功したか？
	bool IsValidProgram();                                                        // vertex,fragmentともにリンク成功してるか？
private:
	// Store the shader object IDs
	GLuint mVertexShader;                                                         // 頂点シェーダーID
	GLuint mFragShader;                                                           // フラグメントシェーダーID
	GLuint mShaderProgram;                                                        // シェーダープログラム（頂点＋フラグメント）ID 
};
