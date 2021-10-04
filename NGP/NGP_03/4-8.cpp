/*
* 명령행 인자로 IP주소로 두 개의 포트 번호( 최소, 최대 )를 입력받아 , 특정 호스트에서 LISTENING 상태인 TCP 포트 번호를 모두 알아내는 프로그램을 작성하라
* 단, 속도가 빠르게 구현할 필요는 없다. 정해진 범위의 포트에 대해 소켓을 생성하고 connenct() 함수를 호출해 성공 여부를 확인하라
* 이러한 프로그램을 포트 스캐너라고함, 공개소프트웨어 nmap참고
*/
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <string>
using namespace std;

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int main(int argc, char* argv[])
{
	

	//명령행 인자 입력은 프로젝트 설정에서 명령인수 지정
	cout << "IP 주소1개, 포트번호(최소, 최대)2개 명령 인수로 입력"<< endl;
	//cout << argv[1]<< endl;
	const char* IP = argv[1];
	int portMin = stoi(argv[2]); //문자열을 int형으로, WORD portMin = (WORD)strtoul() 또는 u_short portMin = (u_short)strtoul()도 사용가능
	int portMax = stoi(argv[3]);

	cout << "Port (" << portMin << " ~ " << portMax << ")" << endl;

	//최소포트부터 최대포트까지 전부 커넥트를 테스트해본다.
	for (int i = portMin; i <= portMax; ++i)
	{
		int retval; // 연결 확인용 리턴값을 받는 변수

		//윈속초기화
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 1;

		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit("socket()");

		SOCKADDR_IN addr;
		ZeroMemory(&addr, sizeof(addr));
		addr.sin_family = AF_INET; //IPV4
		addr.sin_addr.s_addr = inet_addr(IP); //문자열을 32비트 숫자로(네트워크바이트 정렬)
		addr.sin_port = htons(i); //host to network short

		retval = connect(sock, (SOCKADDR*)&addr, sizeof(addr));

		if (retval == SOCKET_ERROR)
		{
			cout << "연결 실패" << endl;
		}
		else if (retval == 0)
		{
			cout << "연결 성공" << endl;
			closesocket(sock);
		}
	}
		WSACleanup();
		return 0;
}