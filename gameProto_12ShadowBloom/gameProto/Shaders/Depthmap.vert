#version 330 core
layout (location = 0) in vec3 aPos      ; // 頂点位置


uniform mat4 uWorldTransform  ; // モデル行列
uniform mat4 lightSpaceMatrix ; // ライト空間行列

void main()
{
    gl_Position = vec4(aPos, 1.0) * uWorldTransform * lightSpaceMatrix ;
}    