
// GLSL 3.3 ��v��
#version 330

// fragment Shader �o�͐�� HDRBuffer��HiBrightBuffer�̓���w��
layout (location = 0) out vec4 HDRBuffer;
layout (location = 1) out vec4 HiBrightBuffer;

// ���_�V�F�[�_�[����̓���
// �e�N�X�`�����W
in vec2 fragTexCoord;
// �@���i���[���h��ԁj
in vec3 fragNormal;
// ���_�ʒu�i���[���h��ԁj
in vec3 fragWorldPos;
// ���C�g��Ԃł̃t���O�����g�ʒu(�V���h�E�}�b�v)
in vec4 fragLightPos;

// �e�N�X�`���T���v�����O
uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;
uniform sampler2D uSpecularMap;
uniform sampler2D uEmissiveMap;

// �f�v�X�}�b�v
uniform sampler2D depthMap;

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

// ���Ȕ������x
uniform float uLuminance;

// �f�B���N�V���i�����C�g
uniform DirectionalLight uDirLight;

// �V���h�E�v�Z
float ShadowCalculation(vec4 fragPosLightSpace);

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

	float shadow = ShadowCalculation(fragLightPos);

	// �e�N�X�`���J���[���f�B�t���[�Y�E�A���r�G���g�J���[�ɍ���
	vec3 texColor = texture(uDiffuseMap, fragTexCoord).rgb;
	vec3 diffuseColor = vec3(Diffuse) * texColor;
	vec3 ambientColor = uAmbientLight * texColor;

	// �V���h�E���������ĕ`��i�A���r�G���g�̓V���h�E��3�����󂯂Ȃ��j
	vec3 result = ( 1.0 - shadow ) * (diffuseColor + Specular) + ambientColor;
	
	//�P�x�v�Z�ɂ���č��P�x�����̂݁AHiBrightBuffer�ɏ�������
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
    {
        HiBrightBuffer = vec4(result, 0.0f) + texture(uEmissiveMap, fragTexCoord) * uLuminance;
    }
    else
    {
        HiBrightBuffer = vec4(0.0f, 0.0f, 0.0f, 0.0f) + texture(uEmissiveMap, fragTexCoord) * uLuminance;
    }
	// HDR�����͂��̂܂�HDR�o�b�t�@�֏o��
    HDRBuffer = vec4(result, 0.0);
}

// �V���h�E

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 0�`1�͈̔͂ɕϊ�
    projCoords = projCoords * 0.5 + 0.5;
    // �f�v�X�}�b�v���[�x�擾 
    float closestDepth = texture(depthMap,projCoords.xy).r;
    // ���݂̃t���O�����g�̃��C�g��Ԃł̐[�x�l�擾
    float currentDepth = projCoords.z;
    // �[�x�o�C�A�X(�f�v�X�}�b�v�𑜓x�ƌ��z
    vec3 normal = normalize(fragNormal);
       vec3 lightDir = normalize(-uDirLight.mDirection);
    float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0001);
    
    // �V���h�E����
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // �f�v�X�}�b�v�͈͊O�͉e�Ȃ��ɂ���
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
    return shadow;
}
