#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;



class Player {
	
public:

	//�޸� ����� Ȯ���ϱ����� �����޽����� ����

	//������
	Player();

	//���������
	Player(const Player& other);

	//�����Ҵ翬����
	Player& operator=(const Player& other);

	// �̵�������
	Player(Player&& other);

	// �̵��Ҵ翬����
	Player& operator=(Player&& other);

	//�Ҹ���
	~Player();

	void Write(ostream& os);
	void Read(ifstream& in);

	string getName() const;

	int getScore() const;

	int getId() const;

	size_t getNum() const;

	

private:

	string name; //�̸�
	int score; //����
	int id; //���̵�
	size_t num; //Ȯ���� �޸� ����Ʈ ��
	char* p; //Ȯ���� �޸� ���۹���

	friend ostream& operator<<(ostream&, const Player& );
	
};


ostream& operator<<(ostream& os, const Player& p);
