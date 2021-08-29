//-------------------------------------------------------------------------------------------------------
//2017.2 C++               월45금56	                    9.4 월요일                  9.1금요일 (1주 2)
//한 학기 강의를 저장할 수 있도록 save()를 만듬
//
//9.2 - 한 학기 강의를 저장할 수 있도록 save()를 만듦.
//9.4 - file 분리,
//    - CPP I/O, cin, cout
//    - 두 변수값의 교환 , pointer
//9.8 - 참조(reference)
//    - namespace, 선언, 메모리 , auto , decltype
//
//9.11 - qsort, 정렬이란 무엇인가? 찾아서 읽어보자.
//       const - ROM
//       void* - 이 번지의 내용은 알아서 타입변환해라
//
// 9.15 - 메모리 구조, 동적할당
//-------------------------------------------------------------------------------------------------------

#include <iostream>
#include "save.h"   //save 함수의 선언은 여기에
#include <stdio.h>



using namespace std;
void save();		// save()라는 함수를 선언(decalraration)


//[문제] 실행 중에 STACK을 넘치게 해보라
//STACK에는 지역변수가들어간다
//그렇다면 지역변수를 계속만들면 Stack이 넘치겠지?

int i;

int main()
{
	
	
	int num;
	cout << ++i << endl;
	main();
	
	
		
	
	system("pause");


	save();
}