#version 330

uniform mat4 modelTransform;
uniform mat4 projectionTransform;
uniform mat4 viewTransform;

in vec3 in_Position;
in vec3 in_Color;

out vec3 ex_Color;


void main()
{
	gl_Position =  projectionTransform * viewTransform * modelTransform * vec4(in_Position.x,in_Position.y,in_Position.z,1.0);
	ex_Color = in_Color;
}