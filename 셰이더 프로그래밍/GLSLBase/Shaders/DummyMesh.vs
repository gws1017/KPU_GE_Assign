#version 450

in vec3 a_Position;

const float PI = 3.141592;

out vec4 v_Color;

uniform float u_Time;

void flag()
{
	vec3 newPos = a_Position;
	float xDis =  a_Position.x + 0.5;
	float Dis = distance(a_Position.xy, vec2(-0.5,0.0));
	float Yvalue = xDis * 0.5 * sin(Dis*2.0 * PI - u_Time);
	float Xvalue = xDis * 0.5 * sin(Dis*2.0 * PI - u_Time);
	newPos.x += Xvalue;
	newPos.y += Yvalue;
	gl_Position = vec4(newPos, 1);

	v_Color = vec4((sin((a_Position.x + 0.5)*2.0 * PI - u_Time)+1.0) / 2.0);
}

void main()
{
	flag();
}
