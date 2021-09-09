#pragma comment(lib,"ws2_32")
#include <winsock2.h>
#include <iostream>

//챕터2 연습문제 1번

using namespace std;
int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		return 1;
	/*MAKEWORD(2, 2) 
	wVersion : 514 //현재 로드한 윈도우즈 소켓의 버전	
	wHighVersion: 514 //로드한 DLL이 지원하는 소켓의 상위버전
	szDescription : WinSock 2.0 //DLL에서 소켓에대한 설명을 기술
	szSystemStatus : Running // 시스템 상태를 알려줌
	*/
	/*MAKEWORD(1, 1)
	wVersion : 257
	wHighVersion: 514
	szDescription : WinSock 2.0
	szSystemStatus : Running
	*/


	cout << "wVersion : " << wsa.wVersion << endl;
	cout << "wHighVersion : " << wsa.wHighVersion << endl;
	cout << "szDescription : " << wsa.szDescription << endl;
	cout << "szSystemStatus : " << wsa.szSystemStatus << endl;

	MessageBox(NULL, "윈속초기화 성공", "알림", MB_OK);
	//윈속 종료
	WSACleanup();
	return 0;
}