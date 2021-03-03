#include "BitmapText.h"
#include "game.h"
#include "Renderer.h"
#include "Texture.h"

const int maxImageNum = 256;

BitmapText::BitmapText()
	: mUseflag(false)
	, mSplitXnum(0)
	, mSplitYnum(0)
	, mSizeX(0)
	, mSizeY(0)
	, mTexture(nullptr)
{
	for (int i = 0; i < maxImageNum; i++)
	{
		mFontMap[i] = -1;
	}
}

BitmapText::~BitmapText()
{
}

void BitmapText::SetFontImage(int xNum, int yNum, const std::string& fontImgfilename)
{
	mTexture = RENDERER->GetTexture(fontImgfilename);
	if (!mTexture)
	{
		return;
	}
	mSplitXnum = xNum;
	mSplitYnum = yNum;
	mSizeX = mTexture->GetWidth()  / mSplitXnum;
	mSizeY = mTexture->GetHeight() / mSplitYnum;

	mUseflag = true;
}

void BitmapText::SetFontColor(int r, int g, int b)
{
}

void BitmapText::TextDraw(int x, int y, const char* str)
{
	//�t�H���g�g�p�\��
	if (!mUseflag)
	{
		return;
	}

	//�e�L�X�g�`��J�n
	int text_x, text_y, idx;
	text_x = x;
	text_y = y;

	while (*str)
	{
		// ���������s�̏ꍇ�͉��s�{�߂�
		if (*str == '\n')
		{
			text_x = x;
			text_y += mSizeY;
			str++;
			continue;
		}
		//�����`��
		idx = GetIndex(*str);
		if (idx >= 0)
		{
			RENDERER->DrawTexture(mTexture, 
				                  idx, 
				                  mSplitXnum,
				                  mSplitYnum,
				                  Vector2(static_cast<float>(text_x), static_cast<float>(text_y)));
		}
		// �����`��ʒu�i�߂�
		text_x += mSizeX;
		str++;
	}
}

// ReMapText�@�摜���ɕ\������Ă��镶����������ɓ���
// �`��ʒu������o�����߂̃C���f�b�N�X�e�[�u�����쐬����֐�
void BitmapText::ReMapText(const char* remapText)
{
	int num = 0;
	const char* p;

	// �����R�[�h���摜�}�b�v�̃C���f�b�N�X�ɕϊ�����e�[�u�������
	// RemapText��"ABC"�Ȃ�
	// mFontMap['A'] = 0;mFontMap['B'] = 1;  �E�E�E�ƂȂ�
	p = remapText;
	char n = 0;
	while (*p && n <= 255)
	{
		mFontMap[*p++] = n++;
	}
}

int BitmapText::GetIndex(char c)
{
	return mFontMap[c];
}
