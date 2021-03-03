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

	unsigned int              mHdrFBO;              // ���������_FBO
	unsigned int              mHdrRBO;
	unsigned int              mHdrColorBuffers[2];

	std::vector<unsigned int> mBlurFBOs;            // �u���[�p�̃t���[���o�b�t�@�I�u�W�F�N�g
	std::vector<unsigned int> mBlurBufferTexs;      // �u���[���ʂ̃e�N�X�`��ID

	const int                 mSampleCount = 15;    // �K�E�X�ڂ����̃T���v�����O�_
	Vector3                   mOffset[15];          // �T���v�����O�_�̍��W(u,v) & w:�d��

};