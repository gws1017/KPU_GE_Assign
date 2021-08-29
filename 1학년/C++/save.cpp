//-------------------------------------------------------------------------------------------------------------------------
//save.cpp - "�ҽ�.cpp"�� �ϵ��ũ�� �����Ѵ�.
//
//2017 09 04 �ۼ�
//-------------------------------------------------------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>

using namespace std; // �Ⱥ��̸� cout endl ifstream ofstream ios~ ��� �ٺٿ��ߵ�

void save()			// save()�� ����(definition)
{
	cout << "���� ������ �����մϴ�" << endl;

	ifstream in("�ҽ�.cpp");				//���� ������ ����
	ofstream out("data.txt", ios::app);
	//�����̱� ���� �� ������ ����.

	// ������ ����� �ð��� ����Ѵ�
	auto tp = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(tp);
	string date = ctime(&t);



	out << endl << endl;
	out << "--------------------------------------------------" << endl;
	out << date;
	out << "--------------------------------------------------" << endl;
	out << endl;

	char c;

	while (in.get(c))
		out.put(c);

}