/*Ŭ�󿡼� ����ڰ� ���� �̸��� �Է� ��(����� ����), �ش� ���� �����͸� �о 
* ������ �����ϴ� ���α׷��� �������� + �������� ������ ���۹������ �����Ͻÿ�. 
* ��, Ŭ���̾�Ʈ�� �����ϴ� ���� �̸��� ������ �����ϴ� �����̸��� ���ƾ� �ϸ�, ���� ����
* �̸��� ���� ������ �ִ� ��쿡 ����°����� ����
*/
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <fstream>
using namespace std;

//Client
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
	cout << "���ϸ� : "<< argv[1] << endl;

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

	retval = connect(clnt_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
	if (retval == SOCKET_ERROR) err_quit("connecnt");

	int fn_len = strlen(argv[1]);
	retval = send(clnt_sock, (char *)&fn_len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	
	retval = send(clnt_sock, argv[1], sizeof(argv[1]), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	cout << "���ϸ� ������" << endl;

	char buf[BSIZE + 1];
	int data_len;
	ifstream in(argv[1],ios::binary);
	while (in.read((char*)buf, sizeof(buf))) {
		data_len = sizeof(buf);
		retval = send(clnt_sock, (char *)&data_len, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		retval = send(clnt_sock, buf, sizeof(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
	}

	closesocket(clnt_sock);
	
	WSACleanup();
	return 0;
}