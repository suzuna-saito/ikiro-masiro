#pragma once
#include <string>

class BitmapText
{
public:
	BitmapText();
	~BitmapText();

	void           SetFontImage(int xNum, int yNum, const std::string& fontimgfilename);                          // �t�H���g�摜���t�H���g���Z�b�g����
	void           SetFontColor(int r, int g, int b);                                                // �t�H���g�J���[�Z�b�g

	void           TextDraw(int x, int y, const char* str);                                             // �e�L�X�g��`��

	void           ReMapText(const char* remapText); // �r�b�g�}�b�v�ɕ���ł��镶��������āA�e�L�X�g���當���������悤�ɂ���B�i�������ŕK�v�j

private:
	int            mFontMap[256];   // �����}�b�v char�^�������t�H���g�C���f�b�N�X�̃}�b�s���O

	unsigned int   mSplitXnum;      // �����}�b�v��������
	unsigned int   mSplitYnum;      // �����}�b�v�c������
	unsigned int   mSizeX;          // 1�������T�C�Yx
	unsigned int   mSizeY;          // 1�������T�C�Yy
	bool           mUseflag;        // �t�H���g�g�p�\�t���O

	int            GetIndex(char c); // �������C���[�W�z��̓Y�����ɕϊ�
	class Texture* mTexture;
};
