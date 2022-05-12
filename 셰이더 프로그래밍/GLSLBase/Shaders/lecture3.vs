#version 450

in vec3 a_Position;
in vec4 a_Color;
in vec3 a_Position2;

uniform float u_time;

out vec4 v_Color;

void main()
{
	vec3 pos = a_Position*(1-u_time) + a_Position2*u_time;
	gl_Position = vec4(pos, 1);
	v_Color = a_Color;
}
