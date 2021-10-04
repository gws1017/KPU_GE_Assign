#include<iostream>
#include<Windows.h>

using namespace std;


BOOL IsLittleEndian() // ��Ʋ����� ���Ľ� 3412 (�������� ��������Ʈ)
{
	WORD w;
	w = 0x1234;
	//cout <<hex<< (int)HIBYTE(w) << endl;
	//cout << (int)LOBYTE(w) << endl;
	//cout << w << endl; //0001 0010 0011 0100
	
	if (HIBYTE(w) == 0x12) // ��Ʋ������̸� ����������Ʈ���� ���ĵǴϱ� WORD�� ���� w�� ��������Ʈ ���� 0x12�� �Էµɰ��̴�.
		return TRUE;
	else
		return FALSE;
}
BOOL IsLBigEndian() // �򿣵�� ���Ľ� 1234 (�������� ��������Ʈ)
{
	WORD w;
	w = 0x1234;
	if (LOBYTE(w) == 0x12) // �򿣵���̸� �ֻ�������Ʈ���� ���ĵǴϱ� WORD�� ���� w�� ��������Ʈ ���� 0x12�� �Էµɰ��̴�.
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