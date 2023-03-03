#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>


using namespace std;
using namespace chrono;

volatile int sum{};

volatile int lock_mem{}; // 0은 아무도 락을 얻지 않았음을 의미
						// 1은 누군가가 락을 얻어서 Critical Section 실행중

bool CAS(volatile int* addr, int expected, int update)
{
	return atomic_compare_exchange_strong(reinterpret_cast<volatile atomic_int*>(addr),
		&expected, update);
}

void cas_lock()
{
	//lock이 1이면 0이 될때까지 기달린다
	//lock이 0이면 atomic하게 1로 바꾸고 리턴한다
	while(CAS(&lock_mem, 0, 1) == false);
	
}

void cas_unlock()
{
	//atomic하게 lock을 다시 0으로 만듬
	CAS(&lock_mem, 1, 0);
}

void worker(int thread_id)
{
	cas_lock();
	for (auto i = 0; i < 50000000/thread_id; ++i) 
		sum = sum + 2;
	cas_unlock();
	
}

int main()
{
	auto start_t = high_resolution_clock::now();
	for (int num_th = 1; num_th <= 8; num_th *= 2) {
	sum = 0;
	vector<thread> threads;
	for (int i = 0; i < num_th; ++i)
		threads.emplace_back(worker, num_th);
	for (auto& th : threads)
		th.join();
	auto end_t = high_resolution_clock::now();
	auto exec_t = end_t - start_t;
	int milli_sec = duration_cast<milliseconds>(exec_t).count();
	cout << "num_th " << num_th << " result : " << sum
		<< " Excute time : " << milli_sec << "ms" << endl;
	}
	
	return 0;
}
