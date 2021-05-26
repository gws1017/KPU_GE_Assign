#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#define EC 11

using namespace std;

vector<vector<pair<int, int>>> edge;
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
bool visited[10001];
int k;


void prim(int v);

int main()
{
	edge.resize(8);				 //정점1 정점2 가중치
	edge[1].push_back({ 1, 2 }); //1 2 1
	edge[2].push_back({ 1, 1 });

	edge[1].push_back({ 6, 3 }); //1 3 6
	edge[3].push_back({ 6, 1 });
	
	edge[2].push_back({ 2, 3 }); //2 3 2
	edge[3].push_back({ 2, 2 });

	edge[2].push_back({ 4, 5 }); //2 5 4
	edge[5].push_back({ 4, 2 });
	
	edge[2].push_back({ 5, 4 }); //2 4 5
	edge[4].push_back({ 5, 2 });

	edge[3].push_back({ 3, 5 }); //3 5 3
	edge[5].push_back({ 3, 3 });

	edge[3].push_back({ 2, 4 }); //3 4 2
	edge[4].push_back({ 2, 3 });
	
	
	edge[4].push_back({ 1, 5 }); //4 5 1
	edge[5].push_back({ 1, 4 });
	
	edge[4].push_back({ 4, 6 }); //4 6 1
	edge[6].push_back({ 4, 4 });

	edge[5].push_back({ 7, 6 }); //5 6 7
	edge[6].push_back({ 7, 5 });

	

	for (int i = 1; i < 8; i++)
	{
		
		for (auto a : edge[i])
			cout << "(" << i << " " << a.second <<") :" << a.first << " ";
		
	}
	cout << endl << endl;
		

	prim(1);
	cout << endl << endl;
	cout << "가중치의 합 : " << k << endl;
	return 0;
}

void prim(int v) {
	visited[v] = true;

	for (auto u : edge[v]) {
		if (!visited[u.second]) {
			pq.push({ u.first,u.second });
		}
	}

	while (!pq.empty()) {
		auto w = pq.top();
		
		pq.pop();
		if (!visited[w.second]) {
			
			cout  << w.first << " ";
			k += w.first;
			prim(w.second);
			return;
		}
	}
	
}