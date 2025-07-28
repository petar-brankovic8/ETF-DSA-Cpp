#include "Graph.h"
#include <fstream>
#include <iostream>

using namespace std;

int Graph::Get_Identificator(string name) {
	//Pomocna metoda za trazenje identifikatora korisnika sa datim imenom binarnom pretragom
	int left = 0, mid = 0, right = n-1;
	while (left <= right) {
		mid = (left + right) / 2;
		if (V[mid] == name) break;
		if (V[mid] > name) right = mid - 1;
		else left = mid + 1;
	}
	if (n == 0 || V[mid] != name) return -1;	//Error: Korisnik sa datim imenom ne postoji
	return mid;
}

int Graph::Load_Graph(ifstream& dat) {
	//Brisanje vec postojeceg grafa radi sprecavanja curenja memorije
	this->Delete_Graph();	

	//Unos broja cvorova 'n'
	if (!(dat >> n)) {	
		n = 0;
		return 1; //Error: Neuspesno citanje broja cvorova 'n'
	}
	if (n > MAXN) {
		n = 0;
		return 2; //Error: Prekoracen maksimalni broj korisnika
	}
	if (n < 0) {
		n = 0;
		return 4; //Error: Negativan broj cvorova n
	}

	//Unos broja grana 'e'
	int e = 0;
	if (!(dat >> e)) return 3; //Error: Neuspesno citanje broja grana e
	if (e < 0) return 5; //Error: Negativan broj grana e

	//Citanje imena korisnika
	for (int i = 0; i < n; i++) {	
		if (!(dat >> V[i])) { 
			n = 0;
			return 6; //Error: Neuspesno citanje imena korisnika
		}
		E[i].next = nullptr;
	}

	//Ucitavanje grana
	for (int i = 0; i < e; i++) {
		//Citanje grane
		string name1, name2;
		double weight;
		if (!(dat >> name1 >> name2 >> weight)) {
			this->Delete_Graph();
			return 7;	//Error: Neuspesno citanje grana
		}

		//Trazenje id-eva unetih korisnika
		int id1 = this->Get_Identificator(name1);
		int id2 = this->Get_Identificator(name2);
		if (id1 == -1 || id2 == -1) {
			this->Delete_Graph();
			return 8;	//Error: Korisnik sa datim imenom u grani ne postoji
		}

		//Pravljenje nove grane
		ListNode* edge = new ListNode();
		edge->node = id2;
		edge->weight = weight;
		edge->next = nullptr;
		
		//Dodavanje nove grane na kraju liste i provera postojanja duplikata grane
		ListNode* it = E[id1].next;
		if (!it) E[id1].next = edge;	//U slucaju prazne liste dodati kao pocetak liste
		else {
			while (it->next && it->node != id2) it = it->next;
			if (it->node == id2) {
				this->Delete_Graph();
				return 9; //Error: Dupla grana izmedju dva korisnika
			}
			it->next = edge;
		}
	}
	return 0;
}

int Graph::Add_Vertice(string name) {
	//Povecanje broja cvorova
	n++;
	if (n >= MAXN) {
		return 1;	//Error: Prekoracen maksimalni broj korisnika
	}

	//Ubacivanje korisnika u leksigorafski sortirani niz imena i pomeranje ostalih
	int i = n - 1;
	while (i > 0 && name < V[i - 1]) {
		V[i] = V[i - 1];
		E[i] = E[i - 1];
		i--;
	}
	V[i] = name;

	//Azuriranje svih grana koje pokazuju na korisnike cija su imena pomerena u nizu
	E[i].next = nullptr;
	for (int j = 0; j < n; j++) {
		ListNode* it = E[j].next;
		while (it) {
			if (it->node >= i) it->node++;	//Ako se nalaze u nizu nakon ubacenog korisnika, njihov id se povecao za jedan
			it = it->next;
		}
	}
	return 0;
}

int Graph::Remove_Vertice(string name) {
	//Trazenje id-a korisnika koji se uklanja
	int id = this->Get_Identificator(name);
	if (id == -1) return 1;	//Error: Korisnik sa datim imenom ne postoji

	//Pomeranje svih korisnika, koji se nalaze desno u nizu u odnosu na unetog korisnika, za jedno mesto u levo
	for (int i = id; i < n-1; i++) {
		V[i] = V[i + 1];
		E[i].next = E[i + 1].next;
	}
	n--;

	//Azuriranje svih grana koje pokazuju na korisnike cija su imena pomerena u nizu
	//Brisanje grana koje pokazuju na uklonjenog korisnika
	for (int i = 0; i < n; i++) {
		ListNode* it = E[i].next;
		ListNode* prev = &E[i];
		while (it) {
			if (it->node > id) {	//Azuriranje
				it->node--;
			}
			else if (it->node == id) {	//Brisanje
				prev->next = it->next;
				delete it;
				it = prev;
			}
			prev = it;
			it = it->next;
		}
	}
	return 0;
}

int Graph::Add_Edge(string name1, string name2, double weight) {
	//Trazenje id-eva unetih korisnika
	int id1 = this->Get_Identificator(name1);
	if (id1 == -1) return 1;	//Error: Korisnik sa datim imenom ne postoji
	int id2 = this->Get_Identificator(name2);
	if (id2 == -1) return 2;	//Error: Korisnik sa datim imenom ne postoji

	//Pravljenje nove grane
	ListNode* edge = new ListNode;
	edge->node = id2;
	edge->weight = weight;
	edge->next = nullptr;

	//Dodavanje nove grane na kraj liste susedstva
	ListNode* it = E[id1].next;
	if (!it) E[id1].next = edge;
	else {
		while (it->next) it = it->next;
		it->next = edge;
	}
	return 0;
}

int Graph::Remove_Edge(string name1, string name2) {
	//Trazenje id-eva unetih korisnika
	int id1 = this->Get_Identificator(name1);
	if (id1 == -1) return 1;	//Error: Korisnik sa datim imenom ne postoji
	int id2 = this->Get_Identificator(name2);
	if (id2 == -1) return 2;	//Error: Korisnik sa datim imenom ne postoji

	//Trazenje grane za uklanjanje
	ListNode* it = E[id1].next, *prev = &E[id1];
	while (it && it->node != id2) {
		prev = it;
		it = it->next;
	}
	if (!it) return 3;	//Error: Trazena grana ne postoji

	//Brisanje grane
	prev->next = it->next;
	delete it;

	return 0;
}

void Graph::Display_Graph() {
	cout << "Broj korisnika: " << n << '\n';
	cout << "Lista korisnika i njihovih konekcija:\n";
	for (int i = 0; i < n; i++) {
		cout << "\t- " << V[i] << ":";
		ListNode* it = E[i].next;
		while (it) {
			cout << ' ' << V[it->node] << '(' << it->weight << ")";
			it = it->next;
		}
		cout << '\n';
	}
}

void Graph::Delete_Graph() {
	//Brisanje svake grane
	for (int i = 0; i < n; i++) {
		ListNode *it = E[i].next, *temp;
		while (it) {
			temp = it;
			it = it->next;
			delete temp;
		}
		E[i].next = nullptr;
	}
	n = 0;
}

int Graph::Like_Post(string name1, string name2) {
	//Trazenje id-eva unetih korisnika
	int id1 = this->Get_Identificator(name1);
	if (id1 == -1) return 1;
	int id2 = this->Get_Identificator(name2);
	if (id2 == -1) return 2;

	//Trazenje grane izmedju unetih korisnika
	ListNode* it = E[id1].next;
	while (it && it->node != id2) it = it->next;
	if (!it || it->node != id2) return 3;	//Error: Trazena grana ne postoji

	//Povecavanje tezine grane
	it->weight += 0.1;
	if (it->weight > 1) it->weight = 1;

	return 0;
}

void Graph::Largest_Component() {
	//Trazenje najvece strogo povezane komponente grafa pomocu Kosaraju-ovog algoritma

	//Inicijalizacija niza posecenosti cvorova i niza za cuvanje vremena izlaza cvora iz DFS-a
	bool* isVisited = new bool[n]();
	struct pair {
		int first;
		int second;
	};
	pair* exitTime = new pair[n];	//exitTime.first - Vreme izlaza cvora iz DFS-a, exitTime.second - za cuvanje ID cvora nakon sortiranja
	int timer = 0;

	//Prvi DFS za zabelezavanje vremena izlaska svakog cvora
	pair* stack = new pair[n*n];	//stack.first - ID cvora, stack.second - Stanje cvora: da li ceka da bude obradjen ili je u obradi i ceka da izadje
	for (int i = 0; i < n; i++) {
		if (isVisited[i])
			continue;

		//Ubacivanje trenutnog cvora u stack
		int topIndex = 0;
		stack[topIndex].first = i;
		stack[topIndex++].second = 0; //Stanje cvora: ulaz = 0, izlaz = 1;

		//DFS iz trenutnog cvora
		while (topIndex) {
			//Stack pop
			int id = stack[--topIndex].first;
			int state = stack[topIndex].second;

			//Obrada novog cvora
			if (state == 0) {
				if (isVisited[id])
					continue;
				isVisited[id] = true;

				//Dodavanje cvora u stack u stanju obrade
				stack[topIndex].first = id;
				stack[topIndex++].second = 1;

				//Prolazak kroz grane i dodavanje cvorova koji treba da budu obradjeni
				ListNode* it = E[id].next;
				while (it) {
					if (!isVisited[it->node]) {
						stack[topIndex].first = it->node;
						stack[topIndex++].second = 0;
					}
					it = it->next;
				}
			}
			//Zavrsena obrada cvora i belezenje vremena izlaza
			else {
				exitTime[id].first = timer++;
				exitTime[id].second = id;
			}
		}
	}
	delete[] stack;

	//Sortiranje niza 'exitTime' opadajuce po vremenu izlaza cvorova iterativnim merge sort-om
	for (int width = 1; width < n; width *= 2) {
		for (int i = 0; i < n; i += 2 * width) {
			int left = i;
			int mid = (i + width < n) ? (i + width) : n;
			int right = (i + 2 * width < n) ? (i + 2 * width) : n;
			//merge
			pair* temp = new pair[2 * width + 1];
			int tempSize = 0;
			int it1 = left, it2 = mid;
			while (it1 < mid && it2 < right) {
				if (exitTime[it1].first >= exitTime[it2].first)
					temp[tempSize++] = exitTime[it1++];
				else
					temp[tempSize++] = exitTime[it2++];
			}
			while (it1 < mid)
				temp[tempSize++] = exitTime[it1++];
			while (it2 < right)
				temp[tempSize++] = exitTime[it2++];
			for (int j = 0; j < tempSize; j++)
				exitTime[left + j] = temp[j];
			delete[] temp;
		}
	}

	//Pravljenje nove liste susedstva 'reverseE' sa okrenutim granama izmedju cvorova
	ListNode* reverseE = new ListNode[n]();
	for (int i = 0; i < n; i++) {
		ListNode* it = E[i].next;
		while (it) {
			ListNode* reverseEdge = new ListNode;
			reverseEdge->node = i;
			reverseEdge->next = reverseE[it->node].next;
			reverseE[it->node].next = reverseEdge;
			it = it->next;
		}
	}

	//Pokazivac na niz koji cuva ID-eve cvorova najvece strogo povezane komponente
	int* largestSCC = nullptr;
	int largestSCC_size = 0;

	//Drugi DFS za trazenje strogo povezanih komponenti
	for (int i = 0; i < n; i++) isVisited[i] = false;
	int* stack2 = new int[n];
	for (int i = 0; i < n; i++) {
		if (isVisited[exitTime[i].second])
			continue;

		//Pravljenje niza za cuvanje cvorova trenutne strogo povezane komponente
		int* SCC = new int[n];
		int SCC_size = 0;
		SCC[SCC_size++] = exitTime[i].second;

		//Ubacivanje trenutnog cvora u stack
		isVisited[exitTime[i].second] = true;
		int topIndex = 0;
		stack2[topIndex++] = exitTime[i].second;

		//DFS iz trenutnog cvora
		while (topIndex) {
			//Stack pop
			int id = stack2[--topIndex];
			ListNode* it = reverseE[id].next;

			//Prolazak kroz grane i dodavanje povezanih cvorova u stack i strogo povezanu komponentu
			while (it) {
				if (!isVisited[it->node]) {
					isVisited[it->node] = true;
					SCC[SCC_size++] = it->node;
					stack2[topIndex++] = it->node;
				}
				it = it->next;
			}
		}

		//Uporedjivanje trenutne strogo povezane komponente i do sad najvece strogo povezane komponente
		if (SCC_size > largestSCC_size) {
			delete largestSCC;
			largestSCC_size = SCC_size;
			largestSCC = SCC;
		}
		else {
			delete[] SCC;
		}
	}

	//Ispis korisnika najvece strogo povezane komponente
	cout << "Velicina najvece komponente: " << largestSCC_size << '\n';
	cout << "Korisnici koji pripadaju najvecoj komponenti:";
	for (int i = 0; i < largestSCC_size; i++) {
		cout << ' ' << V[largestSCC[i]];
	}
	cout << '\n';

	//Brisanje koriscenih nizova
	delete[] isVisited;
	delete[] exitTime;
	delete[] stack2;
	delete[] largestSCC;
	for (int i = 0; i < n; i++) {
		ListNode* it = reverseE[i].next;
		ListNode* prev = &reverseE[i];
		while (it) {
			prev = it;
			it = it->next;
			delete prev;
		}
	}
	delete[] reverseE;
}

int Graph::Shortest_Path(string name1, string name2) {
	//Trazenje najkraceg puta izmedju datih cvorova pomocu Dijkstrinog algoritma

	//Trazenje ID-eva unetih korisnika
	int id1 = this->Get_Identificator(name1);
	if (id1 == -1) return 2; //Error: User1 doesen't exist
	int id2 = this->Get_Identificator(name2);
	if (id2 == -1) return 3; //Error: User2 doesen't exist

	//Implementacija Max Heap-a 
	struct HeapNode {
		int id;
		double dist;
	};

	//Max Heap
	HeapNode* heap = new HeapNode[n];
	int heapSize = 0;

	//Lambda funkcija za zamenu elemenata u heap-u
	auto swap = [](HeapNode& a, HeapNode& b) {
		HeapNode temp = a;
		a = b;
		b = temp;
	};

	//Lambda funkcija za dodavanje novog elementa u heap
	auto push = [&](int id, double dist) {
		heap[heapSize].id = id;
		heap[heapSize].dist = dist;
		int i = heapSize++;
		while (i > 0 && heap[i].dist > heap[(i - 1) / 2].dist) {
			swap(heap[i], heap[(i - 1) / 2]);
			i = (i - 1) / 2;
		}
	};

	//Lambda funckija za uzimanje najveceg elementa i azuriranje heap-a
	auto pop = [&]() -> HeapNode {
		HeapNode root = heap[0];
		heap[0] = heap[--heapSize];

		int i = 0;
		while (true) {
			int largest = i;
			int left = 2 * i + 1;
			int right = 2 * i + 2;

			if (left < heapSize && heap[left].dist > heap[largest].dist)
				largest = left;
			if (right < heapSize && heap[right].dist > heap[largest].dist)
				largest = right;

			if (largest == i)
				break;
			swap(heap[i], heap[largest]);
			i = largest;
		}
		return root;
	};

	//Inicijalizacija nizova koriscenih u Dijkstrinom algoritmu
	double* dist = new double[n];
	bool* isVisited = new bool[n];
	HeapNode* prevId = new HeapNode[n];

	for (int i = 0; i < n; i++) {
		dist[i] = -1;
		isVisited[i] = false;
		prevId[i].id = -1;
		prevId[i].dist = -1;
	}

	//Dodavanje pocetnog cvora u heap
	dist[id1] = 1;
	prevId[id1].id = id1;
	prevId[id1].dist = 1;
	push(id1, 1);

	//Dijkstra
	while (heapSize > 0) {
		//Uzimanje najveceg elementa heap-a
		HeapNode curr = pop();

		if (isVisited[curr.id])
			continue;
		isVisited[curr.id] = true;

		//Obilazak svih povezanih cvorova, dodavanje u heap ako nisu poseceni i azuriranje najkrace distance do cvora
		ListNode* it = E[curr.id].next;
		while (it) {
			if (!isVisited[it->node] && dist[curr.id] * it->weight > dist[it->node]) {
				dist[it->node] = dist[curr.id] * it->weight;
				prevId[it->node].id = curr.id;
				prevId[it->node].dist = dist[it->node];
				push(it->node, dist[it->node]);
			}
			it = it->next;
		}
	}
	if (!isVisited[id2]) return 1; //Error: Ne postoji put izmedju unetih cvorova

	//Rekonstrukcija puta od Korisnika1 do Korisnika2 dodavanjem u stack radi ispisivanja pravilnim redom
	HeapNode* stack = new HeapNode[n];	//stack.id - ID korisnika, stack.dist - Verovatnoca da objava korisnikaID dodje do Korisnika1
	int topIndex = 0;
	int curr = id2;
	while (curr != id1) {
		stack[topIndex].id = curr;
		stack[topIndex++].dist = prevId[curr].dist;
		curr = prevId[curr].id;
	}

	//Ispisivanje najverovatnije puta
	cout << "Najverovatniji put: " << name1 << " - (1)";
	while (topIndex) {
		//Stack pop
		int id = stack[--topIndex].id;
		double weight = stack[topIndex].dist;

		//Ispis trenutnog cvora na putu i verovatnoce da objava trenutnog korisnika dodje do Korisnika1
		cout << " -> " << V[id] << " - (" << weight << ')';
	}
	cout << '\n';

	//Brisanje koriscenih nizova
	delete[] stack;
	delete[] dist;
	delete[] isVisited;
	delete[] prevId;
	delete[] heap;
	return 0;
}

int Graph::K_Popular(int k) {
	//Trazenje k-te najpopularnije osobe tako sto se prvo okrenu sve grane u grafu da bi za svakog korisnika mogao da se vidi njegova pracenost
	//Pa onda racunaju verovatnoce da objava stigne od jednog cvora do drugog izmedju svakog para cvorova pomocu Floyd-ovog algoritma
	//Tada racunamo popularnost svakog korisnika tako sto sabiramo verovatnoce da njegova objava stigne do svakog drugog korisnika
	//popularnost[i] = verovatnoca[i][0] + verovatnoca[i][1] + ... + verovatnoca[i][n-1]
	//Sortiramo korisnike po popularnosti iterativnim merge sort-om i ispisemo k-og najpopularnijeg

	if (k > n || k <= 0) return 1;	//Error: Broj k nije u opsegu [1, n]

	//Pravljenje nove liste susedstva sa okrenutim granama
	ListNode* reverseE = new ListNode[n]();
	for (int i = 0; i < n; i++) {
		ListNode* it = E[i].next;
		while (it) {
			ListNode* reverseEdge = new ListNode;
			reverseEdge->node = i;
			reverseEdge->weight = it->weight;
			reverseEdge->next = reverseE[it->node].next;
			reverseE[it->node].next = reverseEdge;
			it = it->next;
		}
	}

	//Inicijalizacija matrice udaljenosti za Floyd-ov algoritam
	double** dist = new double* [n];
	for (int i = 0; i < n; i++) {
		dist[i] = new double[n];
		for (int j = 0; j < n; j++)
			dist[i][j] = 0;
		dist[i][i] = 1;
		ListNode* it = reverseE[i].next;
		while (it) {
			dist[i][it->node] = it->weight;
			it = it->next;
		}
	}
	
	//Floyd-ov algoritam
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				dist[i][j] = (dist[i][j] > dist[i][k] * dist[k][j]) ? dist[i][j] : (dist[i][k] * dist[k][j]);
			}
		}
	}

	//Racunanje popularnosti svakog cvora
	struct pair {
		int id;
		double weight;
	};
	pair* popularity = new pair[n]();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			popularity[i].weight += dist[i][j];
		popularity[i].id = i;
	}

	//Sortiranje niza 'popularity' opadajuce po popularnosti
	for (int width = 1; width < n; width *= 2) {
		for (int i = 0; i < n; i += 2 * width) {
			int left = i;
			int mid = (i + width < n) ? (i + width) : n;
			int right = (i + 2 * width < n) ? (i + 2 * width) : n;
			//merge
			pair* temp = new pair[2 * width + 1];
			int tempSize = 0;
			int it1 = left, it2 = mid;
			while (it1 < mid && it2 < right) {
				if (popularity[it1].weight >= popularity[it2].weight)
					temp[tempSize++] = popularity[it1++];
				else
					temp[tempSize++] = popularity[it2++];
			}
			while (it1 < mid)
				temp[tempSize++] = popularity[it1++];
			while (it2 < right)
				temp[tempSize++] = popularity[it2++];
			for (int j = 0; j < tempSize; j++)
				popularity[left + j] = temp[j];
			delete[] temp;
		}
	}

	//Ispisivanje k-og najpopularnijeg korisnika
	cout << "K-ti najpopularniji korisnik: " << V[popularity[k-1].id] << "\n";

	//Brisanje koriscenih nizova
	delete[] popularity;
	for (int i = 0; i < n; i++) {
		ListNode* it = reverseE[i].next;
		ListNode* prev = &reverseE[i];
		while (it) {
			prev = it;
			it = it->next;
			delete prev;
		}
	}
	delete[] reverseE;
	for (int i = 0; i < n; i++)
		delete[] dist[i];
	delete[] dist;
	return 0;
}