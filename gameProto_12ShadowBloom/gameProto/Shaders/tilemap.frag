// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// ���_�V�F�[�_�[����̃e�N�X�`�����W����͂Ƃ��Ď󂯎��
in vec2 fragTexCoord;

// �o�̓J���[
out vec4 outColor;

// uniform
uniform sampler2D uTexture;                     // �e�N�X�`��
uniform int       uTileIndex;                   // �`�悵�����^�C���i���o�[
uniform vec2      uTileSetSplitNum;             // �e�N�X�`�����c���������鐔
uniform float     uAlpha;                       // �����x
									            
void main()
{
	ivec2 texSize  = textureSize(uTexture, 0);  // �e�N�X�`���T�C�Y
	ivec2 splitNum = ivec2(uTileSetSplitNum);   // �e�N�X�`��������

	int xpos = uTileIndex % splitNum.x;
	int ypos = uTileIndex / splitNum.x;

	// �uv���Z�o
	vec2 uv = vec2(xpos, ypos) / splitNum;

	// �I�t�Z�b�g�Z�o
	vec2 offset = vec2(1.0f, 1.0f) / splitNum;
	offset = offset * fragTexCoord;            
	
	// �ŏI�e�N�X�`�����W�Z�o
	uv += offset;

	// �e�N�X�`������J���[���T���v�����O
	vec4 color = texture(uTexture, uv);
	color.a = color.a * uAlpha;
    outColor = color;
}
