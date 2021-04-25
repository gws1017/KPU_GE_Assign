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

#define PLAYERCNT 1000000 //�÷��̾���� �鸸�� 
//#define DEBUG
using namespace std;

list<Player> player;

//Player player[PLAYERCNT]; //����vs �����Ҵ� : ������ ������ ũ�Ⱑ Ŀ����. �����Ҵ��� ������ ũ�Ⱑ �۾����´�� �޸�(RAM)�� ����Ѵ�.
//�ٵ� ���� �迭��([]) ���°� ������? ,vector�� array�� ���? �������� []�������� array�����ϼ̴�.
//�׷� �ǹ� array������ vector�� ���°Ը�����? deque ����
//���� �ð��� �纸�Ҵµ� list�� ���� ������. -> array,vector :1.2~2.0 / list 0.6~1.0
//�̸� ������ �ؼ� ��ü�� ���� �����ϰ� ��¸� �ٷιٷ� �ϸ� ���� ���� �� �̴�.

void Answer(int average, int FileSize);
void Answer4();

int main()
{
	_int64 sum{};
	int cnt{};
	ofstream out("����Ʈ ���� 500", ios::binary);
	ifstream in("2021 STL ���� ����", ios::binary);
	for (int i = 0; i < PLAYERCNT; ++i) //
	{
		Player a;
		//player[i].Read(in);
		a.Read(in);
		player.emplace_back(a);
		//sum += player[i].getScore(); //���� �� ���� ������ �� �Ѵ� �ݺ��� �ѹ��� �ϵ���
		sum += a.getScore(); //���� �� ���� ������ �� �Ѵ� �ݺ��� �ѹ��� �ϵ���
		//if (player[i].getNum() == 500)
		if (a.getNum() == 500)
		{
			//player[i].Write(out);
			a.Write(out);
			++cnt; // ������ �Բ� üũ
			//if (cnt >= 988) cout << player[i] << endl;
		}
	}
	//���� ������ �дµ������� �����ڿ� �Ҹ��ڸ� ȣ���
	
	

	//3. Ȯ�ο� 
	out.close();
#ifdef DEBUG
	ifstream in2("����Ʈ ���� 500", ios::binary);
	Player* player2 = new Player[cnt]; //cnt�� ������ƴϴϱ� �����Ҵ�
	for (int i = 0; i < cnt; ++i)
	{
		player2[i].Read(in2);
	}
	cout << player2[0] << endl;
	cout << player2[cnt-1] << endl; 
	// �� ������ 2���� ����ִ� ������? 
	//- ���� �� ���� �߻��Ѵ�? �޸� ������ �ƴѰ� ���� �б��Լ��� �����ߵȰŶ� ��������
	//- ����� ������ ���������� ������ ���� �� ������ �߻��Ѵ�. : 39���� out.close() ---------- �ذ�
	delete[] player2;
#endif

	//4.

	while (1)
	{
		system("cls");
		Answer(sum / PLAYERCNT, (sizeof(Player) + 500) * cnt); // ���� ��� �Լ�
		Answer4();
	}
}

void Answer(int average,int FileSize) {
	//1. ������ �о� ���� ������ ��ü�� ������ ���
	//cout << "1.������ ��ü�� ���� : " << *(end(player) - 1) << endl;
	cout << "1.������ ��ü�� ���� : " << *(--player.end()) << endl;
	cout << endl;

	//2. ��� Player�� ���� ��հ��� ����Ͽ� ����϶�.
	cout << "2. ��� : " << average << endl; // ���� score �ּڰ��� 4989�ε� ����� 427�̴�? -> sum �� �������� �� ������ ����?
	// int ������ ������ ��ü���� ©���� ��� ���� 424�γ����Եȴ� ���� ��ū���� ���� ���ִ� _int64�ڷ����� ���
	// int�� 21�� unsigned int : int21 *2 = 42�� => �� �ڷ�����ε� ����  _int64�� �������ٰ� ������������ ū ũ�⸦ ���� �� ����
	cout << endl;

	//3.
	cout.imbue(locale("en_US")); //���� ũ�⸦ 3�ڸ����� ,������ؼ� ���
	cout << "���� ũ��� : " << FileSize << "KB " << endl;
	cout << endl;
	cout.imbue(locale("C"));
	//�÷��̾� ��ü �ϳ��� ũ�� : sizeof(Player) == 40
	//+�÷��̾� ������ p�� �ڿ����� 500�� => 500+40 = 540
}

void Answer4()
{
	int id, order;
	cout << "id�� �Է��ϼ��� :";
	cin >> id;
	cout << "���� ������ �����ϼ���(1. id ��������, 2.name ��������, 3.score �������� ) : ";
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
		cout << "���Ŀ� �ɸ� �ð�(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
		system("pause");

		//for (int i = 0; i < PLAYERCNT; ++i) {
		//	if (player[i].getId() == id) {
		//		if (i - 1 >= 0) cout << player[i - 1] << endl; //�ش� id�� ù��°�����̰ų� �����������ϰ�� �յ��� �ϳ��� ����Ѵ�
		//		cout << player[i] << endl;
		//		if (PLAYERCNT - 1 >= i + 1)cout << player[i + 1] << endl;
		//		cout << endl;
		//		cout <<"���Ŀ� �ɸ� �ð�(millisecond)"<< chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
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
		cout << "���Ŀ� �ɸ� �ð�(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
		system("pause");

	/*	for (int i = 0; i < PLAYERCNT; ++i) {
			if (player[i].getId() == id) {
				if (i - 1 >= 0) cout << player[i - 1] << endl;
				cout << player[i] << endl;
				if (PLAYERCNT - 1 >= i + 1)cout << player[i + 1] << endl;
				cout << endl;
				cout << "���Ŀ� �ɸ� �ð�(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;

				system("pause");

				break;
			}
		}*/
		break;
	case 3:

		//sort(player.begin(), player.end(), [](const Player& a, const Player& b) { // sort�������� ����� ��? �̵������ڿ� �̵��Ҵ��ڰ� �Ҹ� ->�̵������� ���ǰ� �߸���, �ذ�
		//	return (a.getScore() < b.getScore());
		//	});
		player.sort([](const Player& a, const Player& b) {
			return a.getName().compare(b.getName()) < 0; });
		cout << "���Ŀ� �ɸ� �ð�(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
		system("pause");

		/*for (int i = 0; i < PLAYERCNT; ++i) {
			if (player[i].getId() == id) {
				if (i - 1 >= 0) cout << player[i - 1] << endl;
				cout << player[i] << endl;
				if (PLAYERCNT - 1 >= i + 1)cout << player[i + 1] << endl;
				cout << endl;
				cout << "���Ŀ� �ɸ� �ð�(millisecond)" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;

				system("pause");

				break;
			}
		}*/

		break;
	}
}



