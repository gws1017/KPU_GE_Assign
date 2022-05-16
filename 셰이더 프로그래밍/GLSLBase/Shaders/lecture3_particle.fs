#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_TexCoord;

void main()
{
	float dis = distance(vec2(0,0),v_TexCoord);
	if(dis <0.05f)
	{
		FragColor = v_Color;
	}
	else
	{
		FragColor = vec4(0);
	}
}
