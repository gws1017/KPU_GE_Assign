#include "player.h"

//#define DEBUG

Player::Player() {
#ifdef DEBUGS
	cout << "생성자(int) (this:" << this << ") - 갯수: "
		<< num << ", 위치:" << (void*)p << endl;
#endif // DEBUG
}

Player::Player(const Player& other) : name{ other.name }, score{ other.score }, id{ other.id }, num{ other.num }, p{ new char[num] } 
{
#ifdef DEBUG
	cout << "복사생성자 (this:" << this << ") - 갯수: "
		<< num << ", 위치:" << (void*)p << endl;
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
	cout << "복사할당연산자 (this:" << this << ") - 갯수: "
		<< num << ", 위치:" << (void*)p << endl;
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
	cout << "이동생성자 (this:" << this << ") - 갯수: "
		<< num << ", 위치:" << (void*)p << endl;
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
	cout << "이동할당연산자 (this:" << this << ") - 갯수: "
		<< num << ", 위치:" << (void*)p << endl;
#endif

	return *this;
}

Player::~Player() {
#ifdef DEBUGS
	cout << "소멸자 (this:" << this << ") - 갯수: "
		<< num << ", 위치:" << (void*)p << endl;
#endif // DEBUG
	delete[] p;
}

void Player::Write(ostream& os) 
{
	os.write((char*)this, sizeof(Player));
	os.write((char*) p,num);
}

void Player::Read(ifstream& in) //쓰기가 있으면 읽기도 있어야하지않을까? 그래서 함수로 만듬
{
	in.read((char*)this, sizeof(Player));// 구조는 Write와 똑같다
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
	os << "객체 이름 :" << p.name << ", 아이디 :" << p.id << ", 점수 :" << p.score << ", 자원 수 :" << p.num;

	return os;
}





