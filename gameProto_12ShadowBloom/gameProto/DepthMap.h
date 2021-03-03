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
	class Shader* mDepthShader; //デプスマップ用シェーダー

	unsigned int  mDepthMapFBO; // デプスマップフレームバッファオブジェクト
	unsigned int  mDepthMap; // デプステクスチャ
	unsigned int  mDepthMapSize; // デプスマップサイズ

	Vector3       mLightOrigin; // デプスマップレンダリングの撮影原点
	Vector3       mLightDir; // 光線ベクトル方向
	Matrix4       mLightSpaceMatrix; // ライト空間行列
};