#pragma comment(lib,"ws2_32")
#include <winsock2.h>
#include <iostream>
#include <bitset>
//é��2 �������� 1��

using namespace std;
int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		return 1;
	/*MAKEWORD(2, 2) 
	wVersion : 514 //���� �ε��� �������� ������ ����	
	wHighVersion: 514 //�ε��� DLL�� �����ϴ� ������ ��������
	szDescription : WinSock 2.0 //DLL���� ���Ͽ����� ������ ���
	szSystemStatus : Running // �ý��� ���¸� �˷���
	*/
	/*MAKEWORD(1, 1)
	wVersion : 257
	wHighVersion: 514
	szDescription : WinSock 2.0
	szSystemStatus : Running
	*/

	int major, minor;
	major = LOBYTE(wsa.wVersion); //����8��Ʈ�� �о���� ��ũ�� (�ֹ���)
	minor = HIBYTE(wsa.wVersion); //����8��Ʈ�� �о���� ��ũ�� (�ι���)
	cout << "wVersion : "  << minor << "." << major << endl; //���� ������� ������ ������ ���
	major = LOBYTE(wsa.wHighVersion);
	minor = HIBYTE(wsa.wHighVersion);
	cout << "wHighVersion : " << minor << "." << major  << endl;//���� ��밡���� ������ ���������� ���
	cout << "szDescription : " << wsa.szDescription << endl;
	cout << "szSystemStatus : " << wsa.szSystemStatus << endl;

	MessageBox(NULL, "�����ʱ�ȭ ����", "�˸�", MB_OK);
	//���� ����
	WSACleanup();
	return 0;
}