#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <array>
#include "player.h"

#define PLAYERCNT 1000000 //플레이어수는 백만개 
//#define DEBUG
using namespace std;

array<Player, PLAYERCNT> player;

//Player player[PLAYERCNT]; //전역vs 동적할당 : 전역은 파일의 크기가 커진다. 동적할당은 파일의 크기가 작아지는대신 메모리(RAM)를 사용한다.
//근데 과연 배열을([]) 쓰는게 맞을까? ,vector나 array는 어떤가? 교수님이 []쓰지말고 array쓰라하셨다.
//그럼 의문 array를쓸까 deque, vector를 쓰는게맞을까?


int main()
{
	int sum{};
	int cnt{};
	ofstream out("바이트 수가 500", ios::binary);
	ifstream in("2021 STL 과제 파일", ios::binary);
	for (int i = 0; i < PLAYERCNT; ++i) //
	{
		player[i].Read(in);
		sum += player[i].getScore(); //읽을 때 같이 점수도 더 한다 반복은 한번만 하도록
		if (player[i].getNum() == 500)
		{
			player[i].Write(out);
			++cnt; // 개수도 함께 체크
			//if (cnt >= 988) cout << player[i] << endl;
		}
	}
	//아직 파일을 읽는데까지는 생성자와 소멸자만 호출됨
	
	//1. 파일을 읽어 제일 마지막 객체의 정보를 출력
	cout << "1.마지막 객체의 정보 : " << *(end(player)-1) << endl;
	cout << endl;

	//2. 모든 Player의 점수 평균값을 계산하여 출력하라.
	cout << "2. 평균 : " << sum/PLAYERCNT << endl;
	cout << endl;

	out.close();
	//3. 확인용 
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
#endif
	//플레이어 객체 하나의 크기 : sizeof(Player) == 40
	//+플레이어 내부의 p의 자원수는 500임 => 500+40 = 540

	cout.imbue(locale("en_US"));
	cout << "파일 크기는 : " << (sizeof(Player)+500)*cnt << "KB "  << endl;
	cout << endl;
	cout.imbue(locale("C"));


#ifdef DEBUG
	delete[] player2;
#endif

	//4.
	int id, order;
	while (1)
	{
		cout << "id를 입력하세요 :";
		cin >> id;
		cout << "정렬 순서를 선택하세요(1. id 오름차순, 2.name 오름차순, 3.score 오름차순 ) : ";
		cin >> order;
		cout << endl;
		switch (order) {
			case 1:
				sort(player.begin(), player.end(), [](const Player& a, const Player& b) {
					return a.getId() < b.getId();
					});
				for (int i = 0; i < PLAYERCNT; ++i) {
					if (player[i].getId() == id) {
						if (i - 1 >= 0) cout << player[i-1] << endl; //해당 id가 첫번째인자이거나 마지막인자일경우 앞뒤중 하나만 출력한다
						cout << player[i] << endl;
						if (PLAYERCNT - 1 >= i+1)cout << player[i+1] << endl;
						break;
					}
				}
				break;
			case 2:
				sort(player.begin(), player.end(), [](const Player& a, const Player& b) {
					return a.getName().compare(b.getName()) < 0;
					});
				for (int i = 0; i < PLAYERCNT; ++i) {
					if (player[i].getId() == id) {
						if (i - 1 >= 0) cout << player[i - 1] << endl;
						cout << player[i] << endl;
						if (PLAYERCNT - 1 >= i + 1)cout << player[i + 1] << endl;
						break;
					}
				}
				break;
			case 3:
				cout << "sort start" << endl;
				
				sort(player.begin(), player.end(), [](const Player& a, const Player& b) { // sort과정에서 멈춘다 왜?
					return (a.getScore() < b.getScore());
					});
				cout << "sort end" << endl;

				cout << "search start" << endl;

				for (int i = 0; i < PLAYERCNT; ++i) {
					if (player[i].getId() == id) {
						if (i - 1 >= 0) cout << player[i - 1] << endl;
						cout << player[i] << endl;
						if (PLAYERCNT - 1 >= i + 1)cout << player[i + 1] << endl;
						break;
					}
				}
				cout << "search end" << endl;

				break;
		}
	}
}