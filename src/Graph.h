#pragma once
#include <string.h>
#include <fstream>
#define MAXN 1000

using namespace std;

struct ListNode {
	int node;
	double weight;
	ListNode* next;
};

class Graph
{
public:
	Graph() { n = 0; }
	int Load_Graph(ifstream& dat);
	int Add_Vertice(string name);
	int Remove_Vertice(string name);
	int Add_Edge(string name1, string name2, double weight);
	int Remove_Edge(string name1, string name2);
	void Display_Graph();
	void Delete_Graph();
	int Like_Post(string name1, string name2);
	void Largest_Component();
	int Shortest_Path(string name1, string name2);
	int K_Popular(int k);

protected:
	int Get_Identificator(string name);

private:
	int n;
	string V[MAXN];
	ListNode E[MAXN];
};

