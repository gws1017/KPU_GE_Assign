/*클라에서 사용자가 파일 이름을 입력 시(명령행 인자), 해당 파일 데이터를 읽어서 
* 서버에 전송하는 프로그램을 고정길이 + 가변길이 데이터 전송방식으로 구현하시오. 
* 단, 클라이언트가 전송하는 파일 이름과 서버가 저장하는 파일이름은 같아야 하며, 서버 측에
* 이름이 같은 파일이 있는 경우에 덮어쓰는것으로 가정
*/
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//Client
#define IP "127.0.0.1"
#define PORT 53333
#define BSIZE 2048
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

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	cout << "[" << msg << "] " << lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}

int main(int argc, char* argv[])
{
	cout << "파일명 : "<< argv[1] << endl;

	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	
	SOCKET clnt_sock = socket(AF_INET,SOCK_STREAM,0);
	if (clnt_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serv_addr;
	ZeroMemory(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(PORT);
	cout << "서버 주소 : " << inet_ntoa(serv_addr.sin_addr) << endl;

	cout << "연결 요청중" << endl;
	retval = connect(clnt_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
	if (retval == SOCKET_ERROR) err_quit("connect");
	cout << "연결 완료" << endl;

	int fn_len = strlen(argv[1]);
	retval = send(clnt_sock, (char *)&fn_len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	
	retval = send(clnt_sock, argv[1], sizeof(argv[1]), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	cout << "파일명 전송함" << endl;

	int data_len;
	ifstream in(argv[1],ios::binary);
	if (!in)
	{
		cout << "파일을 열 수 없다" << endl;
	}
	
	vector<char> v(istreambuf_iterator<char>{in}, istreambuf_iterator<char>{});

	char buf[BSIZE + 1];
	ZeroMemory(buf, BSIZE);
	data_len = v.size();
	retval = send(clnt_sock, (char*)&data_len, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		
	}

	cout << "데이터 길이 전송 : " << data_len << endl;

	auto s = v.begin();
	int i = 0;
	int left = data_len;
	int sending;
	while (s < v.end())
	{
		if (i > BSIZE)
		{
			buf[BSIZE] = '\0';
			sending = send(clnt_sock, buf, sizeof(char)* BSIZE, 0);
			if (sending == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
			else if (sending == 0) break;
			cout << "데이터 전송 : " << buf << endl;
			cout << "데이터 전송 남은량 : " << left << endl;
			cout << "데이터 전송량 : " << sending << endl;


			left -= sending;
			++s;
			i = -1;
			ZeroMemory(buf, BSIZE);
		}
		if (i != -1)
		{
			buf[i] = *s;
			++s;
			++i;

		}
		if (i == -1) i = 0;
	}

	if (i != 0)
	{
		retval = send(clnt_sock, buf, sizeof(char)*i, 0);
		if (retval == SOCKET_ERROR)
			err_display("send()");

	}

		

	closesocket(clnt_sock);
	
	WSACleanup();
	return 0;
}