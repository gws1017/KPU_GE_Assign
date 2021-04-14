#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <array>
#include "player.h"

#define PLAYERCNT 1000000 //�÷��̾���� �鸸�� 
//#define DEBUG
using namespace std;

array<Player, PLAYERCNT> player;

//Player player[PLAYERCNT]; //����vs �����Ҵ� : ������ ������ ũ�Ⱑ Ŀ����. �����Ҵ��� ������ ũ�Ⱑ �۾����´�� �޸�(RAM)�� ����Ѵ�.
//�ٵ� ���� �迭��([]) ���°� ������? ,vector�� array�� ���? �������� []�������� array�����ϼ̴�.
//�׷� �ǹ� array������ deque, vector�� ���°Ը�����?


int main()
{
	int sum{};
	int cnt{};
	ofstream out("����Ʈ ���� 500", ios::binary);
	ifstream in("2021 STL ���� ����", ios::binary);
	for (int i = 0; i < PLAYERCNT; ++i) //
	{
		player[i].Read(in);
		sum += player[i].getScore(); //���� �� ���� ������ �� �Ѵ� �ݺ��� �ѹ��� �ϵ���
		if (player[i].getNum() == 500)
		{
			player[i].Write(out);
			++cnt; // ������ �Բ� üũ
			//if (cnt >= 988) cout << player[i] << endl;
		}
	}
	//���� ������ �дµ������� �����ڿ� �Ҹ��ڸ� ȣ���
	
	//1. ������ �о� ���� ������ ��ü�� ������ ���
	cout << "1.������ ��ü�� ���� : " << *(end(player)-1) << endl;
	cout << endl;

	//2. ��� Player�� ���� ��հ��� ����Ͽ� ����϶�.
	cout << "2. ��� : " << sum/PLAYERCNT << endl;
	cout << endl;

	out.close();
	//3. Ȯ�ο� 
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
#endif
	//�÷��̾� ��ü �ϳ��� ũ�� : sizeof(Player) == 40
	//+�÷��̾� ������ p�� �ڿ����� 500�� => 500+40 = 540

	cout.imbue(locale("en_US"));
	cout << "���� ũ��� : " << (sizeof(Player)+500)*cnt << "KB "  << endl;
	cout << endl;
	cout.imbue(locale("C"));


#ifdef DEBUG
	delete[] player2;
#endif

	//4.
	int id, order;
	while (1)
	{
		cout << "id�� �Է��ϼ��� :";
		cin >> id;
		cout << "���� ������ �����ϼ���(1. id ��������, 2.name ��������, 3.score �������� ) : ";
		cin >> order;
		cout << endl;
		switch (order) {
			case 1:
				sort(player.begin(), player.end(), [](const Player& a, const Player& b) {
					return a.getId() < b.getId();
					});
				for (int i = 0; i < PLAYERCNT; ++i) {
					if (player[i].getId() == id) {
						if (i - 1 >= 0) cout << player[i-1] << endl; //�ش� id�� ù��°�����̰ų� �����������ϰ�� �յ��� �ϳ��� ����Ѵ�
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
				
				sort(player.begin(), player.end(), [](const Player& a, const Player& b) { // sort�������� ����� ��?
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