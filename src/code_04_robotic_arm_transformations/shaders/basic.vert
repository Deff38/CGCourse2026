#version 410
in vec2 aPosition;
in vec3 aColor;
out vec3 vColor;

uniform mat4 uM;

void main(void)
{
 gl_Position =  uM * vec4(aPosition, 0.0, 1.0);
 vColor = aColor;
}