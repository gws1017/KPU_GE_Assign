#version 330

in vec3 ex_Color;
out vec4 gl_FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 uv;

uniform vec3 viewPos;        //카메라의 EYE
uniform vec3 lightPos; //--- 조명의 위치
uniform vec3 lightColor; //--- 조명의 색
uniform vec3 objectColor; //--- 객체의 색 근데 나는 ex_Color로 위에서 받아오고잇음 ㅅㄱ
uniform sampler2D outTexture;

uniform int flag;

void main(void)
{
    vec3 ambientLight = vec3(0.3,0.3,0.3); //--- 주변 조명 세기
    vec3 ambient = ambientLight * lightColor; //--- 주변 조명 값
    vec3 normalVector = normalize (Normal);
    vec3 lightDir = normalize(lightPos-FragPos); //--- 표면과 조명의 위치로 조명의 방향을 결정한다.
    float diffuseLight = max(dot(normalVector, lightDir), 0.0); //--- N과 L의 내적 값으로 강도 조절: 음수 방지
    vec3 diffuse = diffuseLight * lightColor; //--- 산란 반사 조명값: 산란반사값 * 조명색상값
    int shininess = 128; //--- 광택 계수
    vec3 viewDir = normalize(viewPos-FragPos); //--- 관찰자의 방향
    vec3 reflectDir = reflect (-lightDir, normalVector); //--- 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산
    float tmp =  max (dot (viewDir, reflectDir), 0.0);
    float specularLight = tmp;//--- V와 R의 내적값으로 강도 조절: 음수 방지
    specularLight = pow(specularLight, shininess); //--- shininess 승을 해주어 하이라이트를 만들어준다.
    vec3 specular = specularLight * lightColor; //--- 거울 반사 조명값: 거울반사값 * 조명색상값
    vec3 result = (ambient + diffuse + specular) * ex_Color; //--- 최종 조명 설정된 픽셀 색상: (주변+산란반사+거울반사조명)*객체 색상

    if(flag == 1)gl_FragColor = vec4(ex_Color,1.0);
	else gl_FragColor = vec4(result,1.0);
	gl_FragColor = texture(outTexture,uv ) * gl_FragColor;
}