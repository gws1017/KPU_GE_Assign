/*Ŭ�󿡼� ����ڰ� ���� �̸��� �Է� ��(����� ����), �ش� ���� �����͸� �о 
* ������ �����ϴ� ���α׷��� �������� + �������� ������ ���۹������ �����Ͻÿ�. 
* ��, Ŭ���̾�Ʈ�� �����ϴ� ���� �̸��� ������ �����ϴ� �����̸��� ���ƾ� �ϸ�, ���� ����
* �̸��� ���� ������ �ִ� ��쿡 ����°����� ����
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
	cout << "���� �ּ� : " << inet_ntoa(serv_addr.sin_addr) << endl;

	cout << "���� ��û��" << endl;
	retval = connect(clnt_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
	if (retval == SOCKET_ERROR) err_quit("connect");
	cout << "���� �Ϸ�" << endl;

	int fn_len = strlen(argv[1]);
	retval = send(clnt_sock, (char *)&fn_len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	
	retval = send(clnt_sock, argv[1], sizeof(argv[1]), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	cout << "���ϸ� ������" << endl;

	int data_len;
	ifstream in(argv[1],ios::binary);
	if (!in)
	{
		cout << "������ �� �� ����" << endl;
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

	cout << "������ ���� ���� : " << data_len << endl;

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
			cout << "������ ���� : " << buf << endl;
			cout << "������ ���� ������ : " << left << endl;
			cout << "������ ���۷� : " << sending << endl;


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