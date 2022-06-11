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

void main()
{
	FragColor = Flip();
	//FragColor = vec4(1,1,1,1);
}
