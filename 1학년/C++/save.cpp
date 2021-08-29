//-------------------------------------------------------------------------------------------------------------------------
//save.cpp - "소스.cpp"를 하드디스크에 저장한다.
//
//2017 09 04 작성
//-------------------------------------------------------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>

using namespace std; // 안붙이면 cout endl ifstream ofstream ios~ 등등 다붙여야됨

void save()			// save()를 정의(definition)
{
	cout << "강의 내용을 저장합니다" << endl;

	ifstream in("소스.cpp");				//읽을 파일을 연다
	ofstream out("data.txt", ios::app);
	//덧붙이기 모드로 쓸 파일을 연다.

	// 파일이 저장된 시간을 기록한다
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