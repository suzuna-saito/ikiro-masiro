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
	bool               Load(const std::string& fileName, class Renderer* renderer);   	// メッシュのロード・アンロード
	void               Unload();                                                         	

	class VertexArray* GetVertexArray() { return mVertexArray; }         // メッシュの頂点配列の取得
	class Texture*     GetTexture(int index);                            // 指定されたインデックスからテクスチャの取得
	int                GetTextureID(TextureStage stage);

	const std::string& GetShaderName() const   { return mShaderName; }     // シェーダー名の取得
	const AABB&        GetCollisionBox() const { return mBox; }          // コリジョンボックスを取得（オブジェクト空間）
	float              GetRadius() const       { return mRadius; }             // バウンディングスフィアの半径を取得
	float              GetSpecPower() const    { return mSpecPower; }
	float              GetLuminace() const     { return mLuminance; }

private:
	AABB                                  mBox;                          // AABBコリジョン
	std::vector<class Texture*>           mTextures;                     // メッシュのテクスチャ
	class VertexArray*                    mVertexArray;                  // メッシュの頂点配列

	std::string                           mShaderName;	                 // メッシュ指定のシェーダー
	float                                 mRadius;	                     // バウンディングスフィアの半径
	float                                 mSpecPower;	                 // 表面のスペキュラー値
	float                                 mLuminance;                    // 自己発光強度(HDR)
	std::unordered_map<TextureStage, int> mStageDefTexture;              // テクスチャステージに割り当てられたテクスチャ

};