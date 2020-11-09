#version 330

uniform mat4 modelTransform;
uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform vec3 Colors;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_uv;

out vec3 FragPos;
out vec3 Normal;
out vec3 ex_Color;
out vec2 uv;

void main()
{
	gl_Position =  projectionTransform * viewTransform * modelTransform * vec4(in_Position.x,in_Position.y,in_Position.z,1.0);
	ex_Color = Colors;
	FragPos = vec3(modelTransform*vec4(in_Position,1.0));
	Normal = in_Normal;
	uv = in_uv;
}