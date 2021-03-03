#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloom1;
uniform sampler2D bloom2;
uniform sampler2D bloom3;
uniform sampler2D bloom4;
uniform sampler2D bloom5;

uniform float exposure;

void main()
{             
    vec3 hdrColor    = texture(scene, TexCoords).rgb;
    vec3 bloomColor1 = texture(bloom1, TexCoords).rgb * 1.0f;
    vec3 bloomColor2 = texture(bloom2, TexCoords).rgb * 2.0f;
    vec3 bloomColor3 = texture(bloom3, TexCoords).rgb * 4.0f;
    vec3 bloomColor4 = texture(bloom4, TexCoords).rgb * 8.0f;
    vec3 bloomColor5 = texture(bloom5, TexCoords).rgb * 16.0f;

    hdrColor += bloomColor1 + bloomColor2 + bloomColor3 + bloomColor4 + bloomColor5; 
  
    // 露出トーンマッピング
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    
    // ガンマコレクション
    //const float gamma = 2.2;
    //mapped = pow(mapped, vec3(1.0 / gamma));  
    FragColor = vec4(mapped, 1.0);
}  