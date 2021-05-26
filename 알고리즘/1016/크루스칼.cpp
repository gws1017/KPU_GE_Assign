#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int check[7];

struct Edge {
	int node[2];
	int distance;

	Edge(int a, int b, int distance)
	{
		this->node[0] = a;
		this->node[1] = b;
		this->distance = distance;
	}

	bool operator<(Edge &edge) {
		return this->distance < edge.distance;
	}

	
	
};
int getParent(int node) {
	if (check[node] == node)return node;
	return getParent(check[node]);
}

void unionParent(int node1, int node2) {
	node1 = getParent(node1);
	node2 = getParent(node2);
	if (node1 < node2) check[node2] = node1;
	else check[node1] = node2;
}

bool isCycle(int node1, int node2) {
	node1 = getParent(node1);
	node2 = getParent(node2);
	if (node1 == node2) return true;
	else return false;
}

int main() {
	vector<Edge> v;

	v.push_back(Edge(1, 7, 10));
	v.push_back(Edge(1, 4, 20));
	v.push_back(Edge(1, 2, 23));
	v.push_back(Edge(2, 3, 36));
	v.push_back(Edge(2, 3, 21));
	v.push_back(Edge(2, 4, 45));
	v.push_back(Edge(3, 5, 28));
	v.push_back(Edge(3, 5, 57));
	v.push_back(Edge(3, 6, 31));
	v.push_back(Edge(4, 7, 60));
	v.push_back(Edge(5, 6, 40));

	sort(v.begin(), v.end());

	for (int i = 1; i <= 7; ++i) check[i] = i;

	cout << "���ĵ� ������ ���� ǥ��" << endl;
	for (int i = 0; i < v.size(); ++i) {

		
		cout << "(" << v[i].node[0] << "," << v[i].node[1] << ") :" << v[i].distance << " ";
		

	}
	cout << endl << endl;

	int sum = 0;
	cout << "�ּ� ����Ʈ��" << endl << endl;
	for (int i = 0; i < v.size(); ++i) {
		
		if (!isCycle(v[i].node[0], v[i].node[1])) {
			cout<<"(" << v[i].node[0] << "," << v[i].node[1] << ") :" <<v[i].distance << " ";
			sum += v[i].distance;
			unionParent(v[i].node[0], v[i].node[1]);
		}
		
	}

	cout << endl;
	cout << "����ġ�� �� : " << sum << endl;
}