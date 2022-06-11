#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexSampler;
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

vec4 P3() //P2 에서 y만 변경하면된다
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	float newValue = abs((v_TexCoord.x-1.0) * 3.0f);
	newTexCoord.x = fract(3.0f*v_TexCoord.x);
	newTexCoord.y = newTexCoord.y/3.0 + floor(newValue) / 3.0;
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P4() //P2 에서 y만 변경하면된다
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.y = fract(3.0f*v_TexCoord.y)/3.0 +floor(abs((v_TexCoord.y - 1.0)*3.0))/3.0;
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

void main()
{
	//FragColor = Flip();
	//FragColor = P1();
	//FragColor = P2();
	//FragColor = P3();
	FragColor = P4();
	//FragColor = vec4(1,1,1,1);
}
