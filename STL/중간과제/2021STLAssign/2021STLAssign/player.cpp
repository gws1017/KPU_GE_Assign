#include "player.h"

//#define DEBUG

Player::Player() {
#ifdef DEBUGS
	cout << "������(int) (this:" << this << ") - ����: "
		<< num << ", ��ġ:" << (void*)p << endl;
#endif // DEBUG
}

Player::Player(const Player& other) : name{ other.name }, score{ other.score }, id{ other.id }, num{ other.num }, p{ new char[num] } 
{
#ifdef DEBUG
	cout << "��������� (this:" << this << ") - ����: "
		<< num << ", ��ġ:" << (void*)p << endl;
#endif
}

Player& Player::operator=(const Player& other) {
	if (this != &other) {
		this->~Player();
		name = other.name;
		score = other.score;
		id = other.id;
		num = other.num;
		p = new char[num];
		memcpy(p, other.p, num);
	}

#ifdef DEBUG
	cout << "�����Ҵ翬���� (this:" << this << ") - ����: "
		<< num << ", ��ġ:" << (void*)p << endl;
#endif

	return *this;
}

Player::Player(Player&& other) : name{ other.name }, score{ other.score }, id{ other.id }, num{ other.num }, p{ new char[num] } {
	p = other.p;
	other.p = nullptr;
	other.num = 0;
	other.id = 0;
	other.score = 0;
	other.name = {};

#ifdef DEBUG
	cout << "�̵������� (this:" << this << ") - ����: "
		<< num << ", ��ġ:" << (void*)p << endl;
#endif
}

Player& Player::operator=(Player&& other) {
	if (this != &other) {
		this->~Player();
		name = other.name;
		score = other.score;
		id = other.id;
		num = other.num;
		p = other.p;

		other.num = 0;
		other.score = 0;
		other.id = 0;
		other.name = {};
		other.p = nullptr;
	}

#ifdef DEBUG
	cout << "�̵��Ҵ翬���� (this:" << this << ") - ����: "
		<< num << ", ��ġ:" << (void*)p << endl;
#endif

	return *this;
}

Player::~Player() {
#ifdef DEBUGS
	cout << "�Ҹ��� (this:" << this << ") - ����: "
		<< num << ", ��ġ:" << (void*)p << endl;
#endif // DEBUG
	delete[] p;
}

void Player::Write(ostream& os) 
{
	os.write((char*)this, sizeof(Player));
	os.write((char*) p,num);
}

void Player::Read(ifstream& in) //���Ⱑ ������ �б⵵ �־������������? �׷��� �Լ��� ����
{
	in.read((char*)this, sizeof(Player));// ������ Write�� �Ȱ���
	p = new char[num];
	in.read((char*)p, num);
	
}

string Player::getName() const {
	return name;
}

int Player::getId() const {
	return id;
}

int Player::getScore() const {
	return score;
}

size_t Player::getNum() const {
	return num;
}

ostream& operator<<(ostream& os, const Player& p)
{
	os << "��ü �̸� :" << p.name << ", ���̵� :" << p.id << ", ���� :" << p.score << ", �ڿ� �� :" << p.num;

	return os;
}





