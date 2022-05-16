#version 450

layout(location=0) out vec4 FragColor;
in vec4 v_Color;


const float PI = 3.141592;

vec4 CrossPattern()
{
	vec4 returnValue = vec4(1,1,1,1);
	float XAxis = sin(10*(v_Color.x * 2 * PI + PI / 4.0f)); // 끝 깔끔 -> 시험 (PI / 4.0f 정답)
	float YAxis = sin(10*(v_Color.y * 2 * PI + PI / 4.0f));
	float resultColor = max(XAxis,YAxis);
	returnValue = vec4(resultColor);
	return returnValue;
}

vec4 DrawCircle()
{
	float dis = distance(v_Color.xy,vec2(0.5,0.5));
	
	vec4 newColor = vec4(0, 0, 0, 0);

	if(dis < 0.5)
	{
		newColor = vec4(1, 1, 1, 1);
	}
	else
	{
		newColor = vec4(0, 0, 0, 0);
	}
	return newColor;
}
vec4 DrawCircleLine()
{
	float dis = distance(v_Color.xy,vec2(0.5,0.5));

	vec4 newColor = vec4(0, 0, 0, 0);

	if(dis > 0.48 && dis < 0.5)
		newColor = vec4(1);
	else
		newColor = vec4(0);

	return newColor;
}

vec4 DrawMultipleCircles()
{
	float dis = distance(v_Color.xy,vec2(0.5,0.5));// 0 ~ 0.5f
	float temp = sin(10 *dis * 4 * PI); //시험 나올듯
	return vec4(temp);
	
}

void main()
{
	//FragColor = CrossPattern();
	//FragColor = DrawCircle();
	//FragColor = DrawCircleLine();
	FragColor = DrawMultipleCircles();
}
