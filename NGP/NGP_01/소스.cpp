#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

//연습문제 챕터2 - 1번
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
	MessageBox(NULL, "윈속초기화 성공","알림", MB_OK);

	WSACleanup();
	return 0;

}