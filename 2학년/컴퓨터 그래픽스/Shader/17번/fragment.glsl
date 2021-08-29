#version 330


out vec4 gl_FragColor;

uniform vec4 outColor;

void main()
{
	gl_FragColor = outColor;
}