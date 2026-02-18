#version 410
layout(location = 0) out vec4 color;
in vec3 vColor;
uniform float uDelta;
void main(void)
{
    color = vec4(vColor+vec3(uDelta,0.0,0.0), 1.0);
}