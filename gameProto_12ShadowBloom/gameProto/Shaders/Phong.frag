// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// GLSL 3.3 ��v��
#version 330

// ���_�V�F�[�_�[����̓���
// �e�N�X�`�����W
in vec2 fragTexCoord;
// �@���i���[���h��ԁj
in vec3 fragNormal;
// ���_�ʒu�i���[���h��ԁj
in vec3 fragWorldPos;

// �o�̓J���[�i�o�̓s�N�Z���J���[�j
out vec4 outColor;

// �e�N�X�`���T���v�����O
uniform sampler2D uDiffuseMap;

// �f�B���N�V���i�����C�g�p�\����
struct DirectionalLight
{
	// ���C�g����
	vec3 mDirection;
	// �f�B�t���[�Y�F
	vec3 mDiffuseColor;
	// �X�y�L�����[�F
	vec3 mSpecColor;
};

// ���C�e�B���O�p�ϐ�
// �J�����ʒu�i���[���h��ԁj
uniform vec3 uCameraPos;
// �|���S���\�ʂ̃X�y�L�����[���x
uniform float uSpecPower;
// �A���r�G���g���C�g�F
uniform vec3 uAmbientLight;

// �f�B���N�V���i�����C�g
uniform DirectionalLight uDirLight;

void main()
{
	// �|���S���\�ʂ̖@���i�t���O�����g�V�F�[�_�[��ŕ�Ԃ���Ă���j
	vec3 N = normalize(fragNormal);
	// �|���S���\�ʂ��烉�C�g�����ւ̃x�N�g��
	vec3 L = normalize(-uDirLight.mDirection);
	// �|���S���\�ʂ���J��������
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// -L �x�N�g���� �@�� N �ɑ΂��Ĕ��˂����x�N�g��R�����߂�
	vec3 R = normalize(reflect(-L, N));

	// �t�H�����ˌv�Z
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);

	vec3 Diffuse;
	vec3 Specular;

	Diffuse = uDirLight.mDiffuseColor * max(NdotL,0.0f);
	Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);

	// Final color is texture color times phong light (alpha = 1)
	outColor = texture(uDiffuseMap, fragTexCoord) * vec4((Diffuse + uAmbientLight),1.0f) + vec4(Specular,1.0f);
}
