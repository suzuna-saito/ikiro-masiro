#version 330 core

out vec4 FragColor;
in  vec2 TexCoords;

uniform sampler2D blursource;

struct GaussBlurParam
{
	int       SampleCount;
	vec3      Offset[16];   
};

uniform GaussBlurParam param;

void main()
{
	vec4 result = vec4(0.0f);
	for(int i = 0; i < param.SampleCount; i++)
	{
		result += param.Offset[i].z * texture( blursource, TexCoords + param.Offset[i].xy );
	}
	result.w = 1.0f;
	FragColor = result;
}
