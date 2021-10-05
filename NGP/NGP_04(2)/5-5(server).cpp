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
	int left = len; //받은 데이터의 남은량을 기록하는 변수
	

	while (left > 0)
	{
		
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;
		else if (received == 0) break;
		left -= received;
		ptr += received;
	}
}

//파일을 받으면서 쓰는 함수
int recvnf(SOCKET s, char* buf, long len, int flags, char* fn)
{
	ofstream out(fn, ios::binary);
	char* ptr=buf;
	long received;
	long left = len; //받은 데이터의 남은량을 기록하는 변수
	long data_len;
	long retval;
	float percent;
	
	while (left > 0)
	{
		percent = (float)(len - left) / (float)len * 100.0f;

		//데이터 길이먼저받아야함
		retval = recvn(s, (char*)&data_len, sizeof(long), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		cout << "data_len : " << data_len << endl;

		received = recv(s, ptr, sizeof(char)* data_len, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;

		cout << "\r전송률 :" << percent;

		out.write((char*)ptr, sizeof(char) * received);
		left -= received;
		ptr += received;
		if (left == 0)
		{
			cout << "\r전송률 :" << percent << endl;
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
	cout << "바인드 성공" << endl;
	retval = listen(serv_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	cout << "리슨 성공" << endl;


	SOCKADDR_IN clnt_addr;
	SOCKET clnt_sock;
	char file_name[100];

	int addr_len;
	int fn_len;


	while (1) {
		addr_len = sizeof(clnt_addr);

		cout << "접속 받는중" << endl;
		clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &addr_len);
		if (clnt_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		cout << "접속 성공" << endl;
		cout << "클라 주소 : " << inet_ntoa(clnt_addr.sin_addr) << endl;

		//파일이름 길이 받기
		retval = recvn(clnt_sock, (char *)&fn_len, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		cout << "file_len : "<< fn_len << endl;
		//파일이름 입력 받기
		retval = recvn(clnt_sock, file_name, fn_len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		file_name[fn_len] = '\0';
		cout << "파일 명 : " << file_name << endl;

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

			//파일쓰기모드로 데이터 받기
			retval = recvnf(clnt_sock, buf, file_size, 0, file_name);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			
	
		
		

		closesocket(clnt_sock);
		cout << endl << "클라이언트 종료" << endl;
	}

	


	
	closesocket(serv_sock);
	WSACleanup();
	return 0;
}