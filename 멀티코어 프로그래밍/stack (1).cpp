#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;
using namespace chrono;

constexpr int MAX_THREADS = 16;

class null_mutex {
public:
	void lock() {}
	void unlock() {}
};

class NODE {
public:
	int v;
	NODE* next;
	NODE() : v(-1), next(nullptr) {}
	NODE(int x) : v(x), next(nullptr) {}
};

class C_STACK {
	NODE* top;
	null_mutex ll;
public:
	C_STACK() : top (nullptr)
	{
	}
	void Push(int x)
	{
		NODE* e = new NODE{ x };
		ll.lock();
		e->next = top;
		top = e;
		ll.unlock();
	}

	int Pop()
	{
		ll.lock();
		if (top == nullptr) {
			ll.unlock();
			return -2;
		}
		int res = top->v;
		NODE* t = top;
		top = top->next;
		ll.unlock();
		delete t;
		return res;
	}

	void print20()
	{
		NODE* p = top;
		for (int i = 0; i < 20; ++i) {
			if (p == nullptr) break;
			cout << p->v << ", ";
			p = p->next;
		}
		cout << endl;
	}

	void clear()
	{
		while (nullptr != top) {
			NODE* t = top;
			top = top->next;
			delete t;
		}
	}
};

class LF_STACK {
	NODE* volatile top;
public:
	LF_STACK() : top(nullptr)
	{
	}

	//bool CAS(NODE* volatile* next, NODE* old_p, NODE* new_p)
	//{
	//	return atomic_compare_exchange_strong(
	//		reinterpret_cast<volatile atomic_llong*>(next),
	//		reinterpret_cast<long long*>(&old_p),
	//		reinterpret_cast<long long>(new_p));
	//}

	bool CAS(NODE* volatile* next, NODE* old_p, NODE* new_p)
	{
		return atomic_compare_exchange_strong(
			reinterpret_cast<volatile atomic_long*>(next),
			reinterpret_cast<long *>(&old_p),
			reinterpret_cast<long >(new_p));
	}

	void Push(int x)
	{
		NODE* e = new NODE{ x };
		while (true) {
			NODE* last = top;
			e->next = last;
			if (true == CAS(&top, last, e))
				return;
		}
	}

	int Pop()
	{
		while (true) {
			NODE* last = top;
			if (last == nullptr)
				return -2;
			NODE* next = last->next;
			int res = last->v;
			if (true == CAS(&top, last, next))
				return res;
		}
	}

	void print20()
	{
		NODE* p = top;
		for (int i = 0; i < 20; ++i) {
			if (p == nullptr) break;
			cout << p->v << ", ";
			p = p->next;
		}
		cout << endl;
	}

	void clear()
	{
		while (nullptr != top) {
			NODE* t = top;
			top = top->next;
			delete t;
		}
	}
};

class BACKOFF {
	int limit;
public:
	BACKOFF() : limit(10) {}
	void backoff()
	{
		int delay = rand() % limit + 1;
		limit = limit * 2;
		if (limit > 200) limit = 200;
		_asm mov eax, delay;
	bo_loop:
		_asm dec eax;
		_asm jnz bo_loop;
	}
};

class LF_BO_STACK {
	NODE* volatile top;
public:
	LF_BO_STACK() : top(nullptr)
	{
	}

	bool CAS(NODE* volatile* next, NODE* old_p, NODE* new_p)
	{
		return atomic_compare_exchange_strong(
			reinterpret_cast<volatile atomic_long*>(next),
			reinterpret_cast<long*>(&old_p),
			reinterpret_cast<long>(new_p));
	}

	void Push(int x)
	{
		NODE* e = new NODE{ x };
		BACKOFF bo;
		while (true) {
			NODE* last = top;
			e->next = last;
			if (true == CAS(&top, last, e))
				return;
			bo.backoff();
		}
	}

	int Pop()
	{
		BACKOFF bo;
		while (true) {
			NODE* last = top;
			if (last == nullptr)
				return -2;
			NODE* next = last->next;
			int res = last->v;
			if (true == CAS(&top, last, next))
				return res;
			bo.backoff();
		}
	}

	void print20()
	{
		NODE* p = top;
		for (int i = 0; i < 20; ++i) {
			if (p == nullptr) break;
			cout << p->v << ", ";
			p = p->next;
		}
		cout << endl;
	}

	void clear()
	{
		while (nullptr != top) {
			NODE* t = top;
			top = top->next;
			delete t;
		}
	}
};

constexpr int EMPTY = 00;
constexpr int WAITING = 01;
constexpr int BUSY = 02;

constexpr int TIME_OUT = 1000;

class EXCHANGER
{
	atomic_int value;		// MSB 2 bit,  00:EMPTY, 01:WAITING, 10:BUSY
public:
	EXCHANGER() : value(0) {}
	int exchange(int x, bool* is_busy)
	{
		for (int j = 0; j < TIME_OUT; ++j) {
			int cur_value = value;
			int state = value >> 30;
			switch (state) {
			case EMPTY: {
				int new_value = (WAITING << 30) | x;
				if (true == atomic_compare_exchange_strong(&value, &cur_value, new_value)) {
					bool success = false;
					for (int i = 0; i < TIME_OUT; ++i)
						if (BUSY == (value >> 30)) {
							success = true;
							break;
						}
					if (success) {
						int ret = value & 0x3FFFFFFF;
						value = 0;
						return ret;
					}
					else {
						if (true == atomic_compare_exchange_strong(&value, &new_value, 0))
							return -1;
						else {
							int ret = value & 0x3FFFFFFF;
							value = 0;
							return ret;
						}
					}
				}
				else continue;
			}
					  break;
			case WAITING: {
				int new_value = (BUSY << 30) | x;
				if (true == atomic_compare_exchange_strong(&value, &cur_value, new_value)) {
					return cur_value & 0x3FFFFFFF;
				}
				else
					continue;
			}
				break;
			case BUSY:
				*is_busy = true;
				continue;
				break;
			}
		}
		*is_busy = true;
		return -2;
	}
};

class EL_ARRAY {
	atomic_int range;
	EXCHANGER ex_array[MAX_THREADS];
public:
	EL_ARRAY() : range(1) {}
	int visit(int x)
	{
		int slot = rand() % range;
		bool busy = false;
		int ret = ex_array[slot].exchange(x, &busy);
		int old_range = range;
		if ((-2 == ret) && (range > 1))
			atomic_compare_exchange_strong(&range, &old_range, old_range - 1);
		if ((true == busy) && (range < MAX_THREADS / 2))
			atomic_compare_exchange_strong(&range, &old_range, old_range + 1);
		return ret;
	}
};

class LF_EL_STACK {
	NODE* volatile top;
	EL_ARRAY el_;
public:
	LF_EL_STACK() : top(nullptr)
	{
	}

	bool CAS(NODE* volatile* next, NODE* old_p, NODE* new_p)
	{
		return atomic_compare_exchange_strong(
			reinterpret_cast<volatile atomic_long*>(next),
			reinterpret_cast<long*>(&old_p),
			reinterpret_cast<long>(new_p));
	}

	void Push(int x)
	{
		NODE* e = new NODE{ x };
		while (true) {
			NODE* last = top;
			e->next = last;
			if (true == CAS(&top, last, e))
				return;
			int ret = el_.visit(x);
			if (-1 == ret) return;
		}
	}

	int Pop()
	{
		BACKOFF bo;
		while (true) {
			NODE* last = top;
			if (last == nullptr)
				return -2;
			NODE* next = last->next;
			int res = last->v;
			if (true == CAS(&top, last, next))
				return res;
			bo.backoff();
		}
	}

	void print20()
	{
		NODE* p = top;
		for (int i = 0; i < 20; ++i) {
			if (p == nullptr) break;
			cout << p->v << ", ";
			p = p->next;
		}
		cout << endl;
	}

	void clear()
	{
		while (nullptr != top) {
			NODE* t = top;
			top = top->next;
			delete t;
		}
	}
};

//C_STACK my_stack;
//LF_STACK my_stack;
LF_BO_STACK my_stack;
constexpr int NUM_TEST = 10000000;

void worker(int threadNum)
{
	for (int i = 1; i < NUM_TEST / threadNum; i++) {
		if ((rand() % 2) || (i < 1000 / threadNum)) {
			my_stack.Push(i);
		}
		else {
			my_stack.Pop();
		}
	}
}


int main()
{
	for (int num_threads = 1; num_threads <= MAX_THREADS; num_threads *= 2) {
		vector <thread> threads;
		my_stack.clear();
		auto start_t = high_resolution_clock::now();
		for (int i = 0; i < num_threads; ++i)
			threads.emplace_back(worker, num_threads);
		for (auto& th : threads)
			th.join();
		auto end_t = high_resolution_clock::now();
		auto exec_t = end_t - start_t;
		auto exec_ms = duration_cast<milliseconds>(exec_t).count();
		my_stack.print20();
		cout << num_threads << " Threads.  Exec Time : " << exec_ms << endl;
	}
}