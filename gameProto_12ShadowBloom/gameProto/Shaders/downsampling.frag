#version 330 core

out vec4 FragColor;
in  vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    ivec2 size = textureSize(screenTexture, 0);
    vec2 tex_offset = vec2( 1.0f / size );

    vec3 c1 = vec3(texture(screenTexture, TexCoords + tex_offset * vec2( 1.0f, 1.0f)));
    vec3 c2 = vec3(texture(screenTexture, TexCoords + tex_offset * vec2(-1.0f, 1.0f)));
    vec3 c3 = vec3(texture(screenTexture, TexCoords + tex_offset * vec2( 1.0f,-1.0f)));
    vec3 c4 = vec3(texture(screenTexture, TexCoords + tex_offset * vec2(-1.0f,-1.0f)));
    vec3 c5 = vec3(texture(screenTexture, TexCoords + tex_offset * vec2( 0.0f, 0.0f)));

    FragColor = vec4((c1 + c2 + c3 + c4 + c5) * 0.2, 1.0f);

}
