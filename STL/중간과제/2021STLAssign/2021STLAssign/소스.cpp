#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <Windows.h>
#include <array>
#include <list>
#include <vector>
#include <chrono>
#include "player.h"

#define PLAYERCNT 1000000 //플레이어수는 백만개 
//#define DEBUG
using namespace std;

list<Player> player;

//Player player[PLAYERCNT]; //전역vs 동적할당 : 전역은 파일의 크기가 커진다. 동적할당은 파일의 크기가 작아지는대신 메모리(RAM)를 사용한다.
//근데 과연 배열을([]) 쓰는게 맞을까? ,vector나 array는 어떤가? 교수님이 []쓰지말고 array쓰라하셨다.
//그럼 의문 array를쓸까 vector를 쓰는게맞을까? deque 사기꾼
//정렬 시간을 재보았는데 list가 가장 빨랐다. -> array,vector :1.2~2.0 / list 0.6~1.0
//미리 정렬을 해서 객체를 각각 정렬하고 출력만 바로바로 하면 가장 빠를 것 이다.

void Answer(int average, int FileSize);
void Answer4();

int main()
{
	_int64 sum{};
	int cnt{};
	ofstream out("바이트 수가 500", ios::binary);
	ifstream in("2021 STL 과제 파일", ios::binary);
	for (int i = 0; i < PLAYERCNT; ++i) //
	{
		Player a;
		//player[i].Read(in);
		a.Read(in);
		player.emplace_back(a);
		//sum += player[i].getScore(); //읽을 때 같이 점수도 더 한다 반복은 한번만 하도록
		sum += a.getScore(); //읽을 때 같이 점수도 더 한다 반복은 한번만 하도록
		//if (player[i].getNum() == 500)
		if (a.getNum() == 500)
		{
			//player[i].Write(out);
			a.Write(out);
			++cnt; // 개수도 함께 체크
			//if (cnt >= 988) cout << player[i] << endl;
		}
	}
	//아직 파일을 읽는데까지는 생성자와 소멸자만 호출됨
	
	

	//3. 확인용 
	out.close();
#ifdef DEBUG
	ifstream in2("바이트 수가 500", ios::binary);
	Player* player2 = new Player[cnt]; //cnt가 상수가아니니까 동적할당
	for (int i = 0; i < cnt; ++i)
	{
		player2[i].Read(in2);
	}
	cout << player2[0] << endl;
	cout << player2[cnt-1] << endl; 
	// 왜 마지막 2개는 비어있는 값이지? 
	//- 읽을 때 문제 발생한다? 메모리 문제는 아닌거 같고 읽기함수도 원래잘된거라 문제없고
	//- 출력후 파일을 닫지않으면 다음에 읽을 때 문제가 발생한다. : 39번줄 out.close() ---------- 해결
	delete[] player2;
#endif

	//4.

	while (1)
	{
		system("cls");
		Answer(sum / PLAYERCNT, (sizeof(Player) + 500) * cnt); // 정답 출력 함수
		Answer4();
	}
}

void Answer(int average,int FileSize) {
	//1. 파일을 읽어 제일 마지막 객체의 정보를 출력
	//cout << "1.마지막 객체의 정보 : " << *(end(player) - 1) << endl;
	cout << "1.마지막 객체의 정보 : " << *(--player.end()) << endl;
	cout << endl;

	//2. 모든 Player의 점수 평균값을 계산하여 출력하라.
	cout << "2. 평균 : " << average << endl; // 문제 score 최솟값이 4989인데 평균은 427이다? -> sum 에 점수합을 다 담을수 없다?
	// int 형으로 담으면 전체합이 짤려서 평균 계산시 424로나오게된다 따라서 더큰수를 담을 수있는 _int64자료형을 사용
	// int형 21억 unsigned int : int21 *2 = 42억 => 이 자료형들로도 부족  _int64는 단위세다가 포기할정도로 큰 크기를 담을 수 있음
	cout << endl;

	//3.
	cout.imbue(locale("en_US")); //파일 크기를 3자리마다 ,찍기위해서 사용
	cout << "파일 크기는 : " << FileSize << "KB " << endl;
	cout << endl;
	cout.imbue(locale("C"));
	//플레이어 객체 하나의 크기 : sizeof(Player) == 40
	//+플레이어 내부의 p의 자원수는 500임 => 500+40 = 540
}

void Answer4()
{
	int id, order;
	cout << "id를 입력하세요 :";
	cin >> id;
	cout << "정렬 순서를 선택하세요(1. id 오름차순, 2.name 오름차순, 3.score 오름차순 ) : ";
	cin >> order;
	cout << endl;
	auto start = chrono::high_resolution_clock::now();
	switch (order) {
	case 1:
		/*sort(player.begin(), player.end(), [](const Player& a, const Player& b) {
			return a.getId() < b.getId();
			});*/
		player.sort([](const Player& a, const Player& b) {
			return a.getId() < b.getId(); });
		cout << "정렬에 걸린 시간(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
		system("pause");

		//for (int i = 0; i < PLAYERCNT; ++i) {
		//	if (player[i].getId() == id) {
		//		if (i - 1 >= 0) cout << player[i - 1] << endl; //해당 id가 첫번째인자이거나 마지막인자일경우 앞뒤중 하나만 출력한다
		//		cout << player[i] << endl;
		//		if (PLAYERCNT - 1 >= i + 1)cout << player[i + 1] << endl;
		//		cout << endl;
		//		cout <<"정렬에 걸린 시간(millisecond)"<< chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
		//		system("pause");
		//		break;
		//	}
		//}
		break;
	case 2:
		/*sort(player.begin(), player.end(), [](const Player& a, const Player& b) {
			return a.getName().compare(b.getName()) < 0;
			});*/
		player.sort([](const Player& a, const Player& b) {
			return a.getName().compare(b.getName()) < 0; });
		cout << "정렬에 걸린 시간(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
		system("pause");

	/*	for (int i = 0; i < PLAYERCNT; ++i) {
			if (player[i].getId() == id) {
				if (i - 1 >= 0) cout << player[i - 1] << endl;
				cout << player[i] << endl;
				if (PLAYERCNT - 1 >= i + 1)cout << player[i + 1] << endl;
				cout << endl;
				cout << "정렬에 걸린 시간(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;

				system("pause");

				break;
			}
		}*/
		break;
	case 3:

		//sort(player.begin(), player.end(), [](const Player& a, const Player& b) { // sort과정에서 멈춘다 왜? 이동생성자와 이동할당자가 불림 ->이동생성자 정의가 잘못됨, 해결
		//	return (a.getScore() < b.getScore());
		//	});
		player.sort([](const Player& a, const Player& b) {
			return a.getName().compare(b.getName()) < 0; });
		cout << "정렬에 걸린 시간(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
		system("pause");

		/*for (int i = 0; i < PLAYERCNT; ++i) {
			if (player[i].getId() == id) {
				if (i - 1 >= 0) cout << player[i - 1] << endl;
				cout << player[i] << endl;
				if (PLAYERCNT - 1 >= i + 1)cout << player[i + 1] << endl;
				cout << endl;
				cout << "정렬에 걸린 시간(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;

				system("pause");

				break;
			}
		}*/

		break;
	}
}



