#version 330

uniform mat4 modelTransform;
uniform mat4 projectionTransform;
uniform mat4 viewTransform;

uniform vec3 outcolor;

in vec3 in_Position;
in vec3 in_Color;

uniform int flag;

out vec3 ex_Color;



void main()
{
	gl_Position =  projectionTransform * viewTransform * modelTransform * vec4(in_Position.x,in_Position.y,in_Position.z,1.0);
	if(flag == 0) ex_Color = in_Color;
	else if(flag == 1) ex_Color = outcolor;
}