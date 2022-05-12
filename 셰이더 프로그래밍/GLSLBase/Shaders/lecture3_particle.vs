#version 450

in vec3 a_Position;
in vec3 a_Velocity;

uniform float u_time;
uniform vec3 u_Accel;
void main()
{
	vec3 newPos;
	float t = u_time;
	float tt = t*t;
	newPos = a_Position + a_Velocity * t + 0.5f * u_Accel * tt;
	gl_Position = vec4(newPos, 1);
}
