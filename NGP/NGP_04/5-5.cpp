/*Ŭ�󿡼� ����ڰ� ���� �̸��� �Է� ��(����� ����), �ش� ���� �����͸� �о 
* ������ �����ϴ� ���α׷��� �������� + �������� ������ ���۹������ �����Ͻÿ�. 
* ��, Ŭ���̾�Ʈ�� �����ϴ� ���� �̸��� ������ �����ϴ� �����̸��� ���ƾ� �ϸ�, ���� ����
* �̸��� ���� ������ �ִ� ��쿡 ����°����� ����
*/
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <filesystem>
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
	cout << "���ϸ� : "<< argv[1] << endl;

	long retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	
	SOCKET serv_sock = socket(AF_INET,SOCK_STREAM,0);
	if (serv_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serv_addr;
	ZeroMemory(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(PORT);
	cout << "���� �ּ� : " << inet_ntoa(serv_addr.sin_addr) << endl;

	cout << "���� ��û��" << endl;
	retval = connect(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
	if (retval == SOCKET_ERROR) err_quit("connect");
	cout << "���� �Ϸ�" << endl;

	int fn_len = strlen(argv[1])+1;
	retval = send(serv_sock, (char *)&fn_len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	
	retval = send(serv_sock, argv[1], fn_len, 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	cout << "���ϸ� ������" << endl;

	long data_len;
	ifstream in(argv[1],ios::binary);
	if (!in)
	{
		cout << "������ �� �� ����" << endl;
	}
	

	char buf[BSIZE];
	ZeroMemory(buf, BSIZE);
	


	in.seekg(0, ios::end);
	long left = in.tellg();
	cout << "left" << left << endl;
	in.seekg(0, ios::beg);

	retval = send(serv_sock, (char*)&left, sizeof(long), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send(file_size)");

	}

	while (1)
	{
		in.read(buf, sizeof(buf));
		data_len = in.gcount();

		retval = send(serv_sock, (char*)&data_len, sizeof(long), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send(data_len)");

		}

		retval = send(serv_sock, buf, sizeof(char) * data_len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
			
		left -= data_len;
		cout << "left ���� : " << left << endl;

		if (left == 0) break;
	}
	


	closesocket(serv_sock);
	
	WSACleanup();
	return 0;
}