#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

//�������� é��2 - 1��
int main()
{
	WSADATA wsa;
	
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		return 1;
	cout << " wVersion :" <<hex << wsa.wVersion << endl;
	cout << " wHighVersion :" << wsa.wHighVersion << endl;
	cout << " szDescription :" << wsa.szDescription << endl;
	cout << " szSystemStatus :" << wsa.szSystemStatus << endl;
	cout << endl;
	MessageBox(NULL, "�����ʱ�ȭ ����","�˸�", MB_OK);

	WSACleanup();
	return 0;

}