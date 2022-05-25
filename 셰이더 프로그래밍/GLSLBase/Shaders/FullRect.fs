#version 450

layout(location=0) out vec4 FragColor;

uniform float u_Time;

in vec2 v_TexCoord;
const float PI = 3.141592;
const float period = 2.f;
const float amp = 0.5f;
void main()
{
	float sinValue = sin(v_TexCoord.x * 2 * PI * period + u_Time) * amp;
	if(v_TexCoord.y * 2.0f - 1.0f < sinValue && v_TexCoord.y * 2.0f - 1.0f > sinValue - 0.008f)
	{
		FragColor = vec4(1);
	}
	else
	{
		FragColor = vec4(0);

	}
	//FragColor = vec4(sinValue,0,0,1);
}
