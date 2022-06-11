#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexSampler;
uniform sampler2D u_TexSampler1;

uniform float u_Time;

in vec2 v_TexCoord;

vec4 Flip()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord = vec2(v_TexCoord.x,1.0 - v_TexCoord.y);
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P1()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.y = abs(2.0f*(v_TexCoord.y -0.5f));
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P2()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.x = fract(3.0f*v_TexCoord.x);
	newTexCoord.y = newTexCoord.y/3.0 + floor(3.0f*v_TexCoord.x) / 3.0;
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P3() //P2 ���� y�� �����ϸ�ȴ�
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	float newValue = abs((v_TexCoord.x-1.0) * 3.0f);
	newTexCoord.x = fract(3.0f*v_TexCoord.x);
	newTexCoord.y = newTexCoord.y/3.0 + floor(newValue) / 3.0;
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P4() // y�� �����ϸ�ȴ�
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.y = fract(3.0f*v_TexCoord.y)/3.0 + floor(abs((v_TexCoord.y - 1.0)*3.0))/3.0;
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P5() 
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.x = fract(2.0f*v_TexCoord.x) + floor(2.0f*v_TexCoord.y)*0.5;
	newTexCoord.y = fract(2.0f*v_TexCoord.y);
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P6() // ���� - ����ȭ �ؼ� ���� �ٲٸ� nxn�� ��� �׸� �� �ְ� �����
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	float i = 4.0;
	float j = 4.0;
	newTexCoord.x = fract(i*v_TexCoord.x) + floor(j*v_TexCoord.y ) * 0.5f;
	newTexCoord.y = fract(j*v_TexCoord.y);
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P7() //���� - x�� �ƴ϶� y�� �������� 0.5 shift �ϱ�
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.x = fract(2.0f*v_TexCoord.x);
	newTexCoord.y = fract(2.0f*v_TexCoord.y)+ floor(2.0f*v_TexCoord.x)*0.5;
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P8() 
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.x = fract(2.0f*v_TexCoord.x);

	if(v_TexCoord.x > 0.5)
	{
		returnValue = texture(u_TexSampler1,newTexCoord);
	}
	else
	{
		returnValue = texture(u_TexSampler,newTexCoord);
	}

	return returnValue;
}

vec4 P9() 
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	vec2 newTexCoord1 = v_TexCoord;
	
	newTexCoord1.x += u_Time;
	//newTexCoord1.x = fract(newTexCoord1.x);
	returnValue = texture(u_TexSampler,newTexCoord) * texture(u_TexSampler1,newTexCoord1);

	return returnValue;
}

vec4 P10() //���� �ؽ�ó ��� �ִϸ��̼�
{
	float texture_index = 6.0;
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.y = (floor(u_Time) + (1.0 - v_TexCoord.y))/texture_index;
	
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

void main()
{
	//FragColor = Flip();
	//FragColor = P1();
	//FragColor = P2();
	//FragColor = P3();
	//FragColor = P4();
	//FragColor = P5();
	//FragColor = P6();
	//FragColor = P7();
	//FragColor = P9();
	FragColor = P10();
	//FragColor = vec4(1,1,1,1);
}
