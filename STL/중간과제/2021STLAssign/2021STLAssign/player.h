#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;



class Player {
	
public:

	//메모리 사용을 확인하기위해 관찰메시지를 구현

	//생성자
	Player();

	//복사생성자
	Player(const Player& other);

	//복사할당연산자
	Player& operator=(const Player& other);

	// 이동생성자
	Player(Player&& other);

	// 이동할당연산자
	Player& operator=(Player&& other);

	//소멸자
	~Player();

	void Write(ostream& os);
	void Read(ifstream& in);

	string getName() const;

	int getScore() const;

	int getId() const;

	size_t getNum() const;

	

private:

	string name; //이름
	int score; //점수
	int id; //아이디
	size_t num; //확보한 메모리 바이트 수
	char* p; //확보한 메모리 시작번지

	friend ostream& operator<<(ostream&, const Player& );
	
};


ostream& operator<<(ostream& os, const Player& p);
