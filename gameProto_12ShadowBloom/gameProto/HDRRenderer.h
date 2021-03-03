#pragma once
#include "Game.h"
#include <vector>

class HDRRenderer
{
public:
	HDRRenderer(int fbowidth, int fboheight, int bloomLevel);
	~HDRRenderer();

	void HdrRecordBegin();
	void HdrRecordEnd();
	void HiBrightBlurCreate();
	void HdrTonemapAndBrightBlurCombine();
	void CopyDepthToScreen();

	unsigned int GetColorTexID() { return mHdrColorBuffers[0]; }
	unsigned int GetHDRTexID() { return mHdrColorBuffers[1]; }

private:
	void  InitHDRBuffers();
	void  InitBlurBuffers();
	void  InitScreenQuadVAO();
	float GaussianDistribution(const Vector2& pos, float rho);
	void  CalcGaussBlurParam(int w, int h, Vector2 dir, float deviation);

	class Shader*             mGaussianBlurShader;
	class Shader*             mDownSamplingShader;
	class Shader*             mHdrToneAndBlurBlendShader;
	unsigned int              mQuadScreenVAO;
	unsigned int              mQuadScreenVBO;

	unsigned int              mBufferWidth;
	unsigned int              mBufferHeight;
	unsigned int              mBloomBufferLevel;
	unsigned int              mBloomBufferNum;

	unsigned int              mHdrFBO;              // 浮動小数点FBO
	unsigned int              mHdrRBO;
	unsigned int              mHdrColorBuffers[2];

	std::vector<unsigned int> mBlurFBOs;            // ブラー用のフレームバッファオブジェクト
	std::vector<unsigned int> mBlurBufferTexs;      // ブラー結果のテクスチャID

	const int                 mSampleCount = 15;    // ガウスぼかしのサンプリング点
	Vector3                   mOffset[15];          // サンプリング点の座標(u,v) & w:重み

};