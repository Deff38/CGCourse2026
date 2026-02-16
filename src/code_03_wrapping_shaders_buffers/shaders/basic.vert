#version 410
in vec2 aPosition;
in vec3 aColor;
out vec3 vColor;
uniform float uDelta;
void main(void)
{
 gl_Position = vec4(aPosition+vec2(uDelta,0.0), 0.0, 1.0);
 vColor = aColor;
}