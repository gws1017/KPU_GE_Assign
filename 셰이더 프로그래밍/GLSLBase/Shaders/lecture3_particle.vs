#version 450

in vec3 a_Position;
in vec3 a_Velocity;

uniform float u_time;

void main()
{
	vec3 newPos;
	newPos = a_Position + u_time * a_Velocity;
	gl_Position = vec4(newPos, 1);
}
