#version 450

layout(location=0) out vec4 FragColor;
in vec4 v_Color;


const float PI = 3.141592;

uniform vec3 u_Points[10];
uniform float u_Time;

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

vec4 DrawCircles()
{
//어느정도 그리면 사라지게 만들어보기
	vec4 returnColor = vec4(0);
	for(int i=0; i<10; i++)
	{
		float d = distance(u_Points[i].xy,v_Color.xy);
		float temp = sin(10*d*4*PI - u_Time*100);
		if( d < u_Time && d < 0.05f )
			returnColor += vec4(temp);
		//if(d <0.1)
		//	returnColor = vec4(1);
	}
	
	return returnColor;
}

vec4 RaidarCircle()
{
//좀더 이쁘게 튜닝해보기
	float d = distance(vec2(0.5,0),v_Color.xy);
	float sinValue = sin(d*2*PI - u_Time * 100);
	sinValue = clamp(pow(sinValue,4),0,1);

	vec4 returnColor = vec4(0.5*sinValue);

	for(int i=0; i<10; i++)
	{
		float dTemp = distance(u_Points[i].xy,v_Color.xy);
		if(dTemp < 0.1)
			returnColor += vec4(0,20*sinValue*(0.1-dTemp),0,0);
	}
	return returnColor;
}

void main()
{
	//FragColor = CrossPattern();
	//FragColor = DrawCircle();
	//FragColor = DrawCircleLine();
	//FragColor = DrawMultipleCircles();
	//FragColor = DrawCircles();
	FragColor = RaidarCircle();
}
