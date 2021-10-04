/*
* ����� ���ڷ� IP�ּҷ� �� ���� ��Ʈ ��ȣ( �ּ�, �ִ� )�� �Է¹޾� , Ư�� ȣ��Ʈ���� LISTENING ������ TCP ��Ʈ ��ȣ�� ��� �˾Ƴ��� ���α׷��� �ۼ��϶�
* ��, �ӵ��� ������ ������ �ʿ�� ����. ������ ������ ��Ʈ�� ���� ������ �����ϰ� connenct() �Լ��� ȣ���� ���� ���θ� Ȯ���϶�
* �̷��� ���α׷��� ��Ʈ ��ĳ�ʶ����, ��������Ʈ���� nmap����
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
	

	//����� ���� �Է��� ������Ʈ �������� ����μ� ����
	cout << "IP �ּ�1��, ��Ʈ��ȣ(�ּ�, �ִ�)2�� ��� �μ��� �Է�"<< endl;
	//cout << argv[1]<< endl;
	const char* IP = argv[1];
	int portMin = stoi(argv[2]); //���ڿ��� int������, WORD portMin = (WORD)strtoul() �Ǵ� u_short portMin = (u_short)strtoul()�� ��밡��
	int portMax = stoi(argv[3]);

	cout << "Port (" << portMin << " ~ " << portMax << ")" << endl;

	//�ּ���Ʈ���� �ִ���Ʈ���� ���� Ŀ��Ʈ�� �׽�Ʈ�غ���.
	for (int i = portMin; i <= portMax; ++i)
	{
		int retval; // ���� Ȯ�ο� ���ϰ��� �޴� ����

		//�����ʱ�ȭ
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 1;

		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit("socket()");

		SOCKADDR_IN addr;
		ZeroMemory(&addr, sizeof(addr));
		addr.sin_family = AF_INET; //IPV4
		addr.sin_addr.s_addr = inet_addr(IP); //���ڿ��� 32��Ʈ ���ڷ�(��Ʈ��ũ����Ʈ ����)
		addr.sin_port = htons(i); //host to network short

		retval = connect(sock, (SOCKADDR*)&addr, sizeof(addr));

		if (retval == SOCKET_ERROR)
		{
			cout << "���� ����" << endl;
		}
		else if (retval == 0)
		{
			cout << "���� ����" << endl;
			closesocket(sock);
		}
	}
		WSACleanup();
		return 0;
}