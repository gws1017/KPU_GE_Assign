#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>


using namespace std;
using namespace chrono;

volatile int sum{};

volatile int lock_mem{}; // 0�� �ƹ��� ���� ���� �ʾ����� �ǹ�
						// 1�� �������� ���� �� Critical Section ������

bool CAS(volatile int* addr, int expected, int update)
{
	return atomic_compare_exchange_strong(reinterpret_cast<volatile atomic_int*>(addr),
		&expected, update);
}

void cas_lock()
{
	//lock�� 1�̸� 0�� �ɶ����� ��޸���
	//lock�� 0�̸� atomic�ϰ� 1�� �ٲٰ� �����Ѵ�
	while(CAS(&lock_mem, 0, 1) == false);
	
}

void cas_unlock()
{
	//atomic�ϰ� lock�� �ٽ� 0���� ����
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
