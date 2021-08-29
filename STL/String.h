//-----------------------------------------------------------------------------
// String.h - STL 내부 동작을 관찰하기 위해 만든 자원을 확보하는 클래스
//
// 2021. 3. 30									Programmed by wulong
//-----------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <random>

using std::cout;
using std::endl;

// #define 관찰			// 이 주석을 풀면 special member의 동작을 알 수 있다.

std::default_random_engine dre;
std::uniform_int_distribution<> uidAlpha{ 'a', 'z' };

class String {
public:
	String(size_t n): num{ n }, p{ new char[num] } {
		#ifdef 관찰
			cout << "생성자(int) (this:" << this << ") - 갯수: "
				<< num << ", 위치:" << (void*)p << endl;
		#endif

		for ( int i = 0; i < num; ++i )
			p[i] = uidAlpha(dre);
	}

	~String() {
		#ifdef 관찰
			cout << "소멸자 (this:" << this << ") - 갯수: "
				<< num << ", 위치:" << (void*)p << endl;
		#endif
		delete[] p;
	}

	// 복사생성자
	String(const String& other) : num{ other.num }, p{ new char[num] } {
		memcpy(p, other.p, num);

		#ifdef 관찰
			cout << "복사생성자 (this:" << this << ") - 갯수: "
				<< num << ", 위치:" << (void*)p << endl;
		#endif
	}

	// 복사할당연산자
	String& operator=(const String& other) {
		if ( this != &other ) {
			this->~String();
			num = other.num;
			p = new char[num];
			memcpy(p, other.p, num);
		}

		#ifdef 관찰
			cout << "복사할당연산자 (this:" << this << ") - 갯수: "
				<< num << ", 위치:" << (void*)p << endl;
		#endif

		return *this;
	}

	// 이동생성자
	String(String&& other) : num{ other.num } {
		p = other.p;
		other.p = nullptr;
		other.num = 0;

		#ifdef 관찰
			cout << "이동생성자 (this:" << this << ") - 갯수: "
				<< num << ", 위치:" << (void*)p << endl;
		#endif
	}

	// 이동할당연산자
	String& operator=(String&& other) {
		if ( this != &other ) {
			this->~String();
			num = other.num;
			p = other.p;

			other.num = 0;
			other.p = nullptr;
		}

		#ifdef 관찰
			cout << "이동할당연산자 (this:" << this << ") - 갯수: "
				<< num << ", 위치:" << (void*)p << endl;
		#endif

		return *this;
	}

	size_t size() const {
		return num;
	}

private:
	size_t num;							// 확보한 자원의 수
	char* p;							// 확보한 자원의 위치

	friend std::ostream& operator<<(std::ostream&, const String&);
};

std::ostream& operator<<(std::ostream& os, const String& s)
{
	for ( int i = 0; i < s.num; ++i )
		os << s.p[i];
	return os;
}



