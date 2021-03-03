#version 330 core
layout (location = 0) in vec3 aPos      ; // ���_�ʒu


uniform mat4 uWorldTransform  ; // ���f���s��
uniform mat4 lightSpaceMatrix ; // ���C�g��ԍs��

void main()
{
    gl_Position = vec4(aPos, 1.0) * uWorldTransform * lightSpaceMatrix ;
}    