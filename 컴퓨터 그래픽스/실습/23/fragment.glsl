#version 330

in vec3 ex_Color;
out vec4 gl_FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 uv;

uniform vec3 viewPos;        //ī�޶��� EYE
uniform vec3 lightPos; //--- ������ ��ġ
uniform vec3 lightColor; //--- ������ ��
uniform vec3 objectColor; //--- ��ü�� �� �ٵ� ���� ex_Color�� ������ �޾ƿ������� ����
uniform sampler2D outTexture;

uniform int flag;

void main(void)
{
    vec3 ambientLight = vec3(0.3,0.3,0.3); //--- �ֺ� ���� ����
    vec3 ambient = ambientLight * lightColor; //--- �ֺ� ���� ��
    vec3 normalVector = normalize (Normal);
    vec3 lightDir = normalize(lightPos-FragPos); //--- ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.
    float diffuseLight = max(dot(normalVector, lightDir), 0.0); //--- N�� L�� ���� ������ ���� ����: ���� ����
    vec3 diffuse = diffuseLight * lightColor; //--- ��� �ݻ� ����: ����ݻ簪 * �������
    int shininess = 128; //--- ���� ���
    vec3 viewDir = normalize(viewPos-FragPos); //--- �������� ����
    vec3 reflectDir = reflect (-lightDir, normalVector); //--- �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ���
    float tmp =  max (dot (viewDir, reflectDir), 0.0);
    float specularLight = tmp;//--- V�� R�� ���������� ���� ����: ���� ����
    specularLight = pow(specularLight, shininess); //--- shininess ���� ���־� ���̶���Ʈ�� ������ش�.
    vec3 specular = specularLight * lightColor; //--- �ſ� �ݻ� ����: �ſ�ݻ簪 * �������
    vec3 result = (ambient + diffuse + specular) * ex_Color; //--- ���� ���� ������ �ȼ� ����: (�ֺ�+����ݻ�+�ſ�ݻ�����)*��ü ����

    if(flag == 1)gl_FragColor = vec4(ex_Color,1.0);
	else gl_FragColor = vec4(result,1.0);
	gl_FragColor = texture(outTexture,uv ) * gl_FragColor;
}