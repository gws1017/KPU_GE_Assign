#include<iostream>
#include<Windows.h>

using namespace std;


BOOL IsLittleEndian() // 리틀엔디안 정렬시 3412 (오른쪽이 상위바이트)
{
	WORD w;
	w = 0x1234;
	//cout <<hex<< (int)HIBYTE(w) << endl;
	//cout << (int)LOBYTE(w) << endl;
	//cout << w << endl; //0001 0010 0011 0100
	
	if (HIBYTE(w) == 0x12) // 리틀엔디안이면 최하위바이트부터 정렬되니까 WORD형 변수 w의 상위바이트 값이 0x12로 입력될것이다.
		return TRUE;
	else
		return FALSE;
}
BOOL IsLBigEndian() // 빅엔디안 정렬시 1234 (오른쪽이 상위바이트)
{
	WORD w;
	w = 0x1234;
	if (LOBYTE(w) == 0x12) // 빅엔디안이면 최상위바이트부터 정렬되니까 WORD형 변수 w의 하위바이트 값이 0x12로 입력될것이다.
		return TRUE;
	else
		return FALSE;
}

int main()
{
	if (IsLittleEndian())
	{
		cout << "LittleEndian " << endl;
	}
	if (IsLBigEndian())
	{
		cout << "BigEndian " << endl;
	}
	
	
}