#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <fstream>
using namespace std;

//SERVER
#define IP "172.30.1.56"
#define PORT 5333
#define BSIZE 512
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
	//cout << "파일명 : " << argv[1] << endl;

	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == INVALID_SOCKET) err_quit("serv_socket()");

	SOCKADDR_IN serv_addr;
	ZeroMemory(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);

	retval = bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(serv_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKADDR_IN clnt_addr;
	SOCKET clnt_sock;
	char buf[BSIZE + 1];
	char file_name[100];
	int addr_len;
	int fn_len;

	while (1) {
		addr_len = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &addr_len);
		if (clnt_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		retval = recv(clnt_sock, (char *)&fn_len, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}

		retval = recv(clnt_sock, file_name, fn_len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}

		int data_len;
		char buf[BSIZE + 1];
		ofstream out(file_name, ios::binary);

		//데이터 길이먼저받아야함
		retval = recv(clnt_sock, (char *)&data_len, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}

		while ( != 0)
		{
			out.write((char*)buf, sizeof(buf));
		}

		closesocket(clnt_sock);
		cout << "클라이언트 종료" << endl;
	}

	//retval = connect(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
	//if (retval == SOCKET_ERROR) err_quit("connect()");

	


	
	closesocket(serv_sock);
	WSACleanup();
	return 0;
}