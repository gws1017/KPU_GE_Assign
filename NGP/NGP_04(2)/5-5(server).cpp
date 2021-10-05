#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <fstream>
using namespace std;

//SERVER
#define IP "172.30.1.56"
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

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len; //���� �������� �������� ����ϴ� ����
	

	while (left > 0)
	{
		
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;
		else if (received == 0) break;
		left -= received;
		ptr += received;
	}
}

//������ �����鼭 ���� �Լ�
int recvnf(SOCKET s, char* buf, long len, int flags, char* fn)
{
	ofstream out(fn, ios::binary);
	char* ptr=buf;
	long received;
	long left = len; //���� �������� �������� ����ϴ� ����
	long data_len;
	long retval;
	float percent;
	
	while (left > 0)
	{
		percent = (float)(len - left) / (float)len * 100.0f;

		//������ ���̸����޾ƾ���
		retval = recvn(s, (char*)&data_len, sizeof(long), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		cout << "data_len : " << data_len << endl;

		received = recv(s, ptr, sizeof(char)* data_len, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;

		cout << "\r���۷� :" << percent;

		out.write((char*)ptr, sizeof(char) * received);
		left -= received;
		ptr += received;
		if (left == 0)
		{
			cout << "\r���۷� :" << percent << endl;
			break;
		}
	}
	

}

int main(int argc, char* argv[])
{

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
	cout << "���ε� ����" << endl;
	retval = listen(serv_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	cout << "���� ����" << endl;


	SOCKADDR_IN clnt_addr;
	SOCKET clnt_sock;
	char file_name[100];

	int addr_len;
	int fn_len;


	while (1) {
		addr_len = sizeof(clnt_addr);

		cout << "���� �޴���" << endl;
		clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &addr_len);
		if (clnt_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		cout << "���� ����" << endl;
		cout << "Ŭ�� �ּ� : " << inet_ntoa(clnt_addr.sin_addr) << endl;

		//�����̸� ���� �ޱ�
		retval = recvn(clnt_sock, (char *)&fn_len, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		cout << "file_len : "<< fn_len << endl;
		//�����̸� �Է� �ޱ�
		retval = recvn(clnt_sock, file_name, fn_len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		file_name[fn_len] = '\0';
		cout << "���� �� : " << file_name << endl;

		long file_size;
		char buf[BSIZE ];
		ZeroMemory(buf, BSIZE);

		retval = recvn(clnt_sock, (char*)&file_size, sizeof(long), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		cout << "file_size : " << file_size << endl;

			//���Ͼ������ ������ �ޱ�
			retval = recvnf(clnt_sock, buf, file_size, 0, file_name);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			
	
		
		

		closesocket(clnt_sock);
		cout << endl << "Ŭ���̾�Ʈ ����" << endl;
	}

	


	
	closesocket(serv_sock);
	WSACleanup();
	return 0;
}