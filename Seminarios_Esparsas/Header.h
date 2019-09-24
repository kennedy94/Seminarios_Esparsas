#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

//estrutura para guardar um arco de um grafo
struct arco {
	//dados do arco
	int i, j;
	float v;
	//construtor do arco
	arco(int i1, int i2, float i3) {
		i = i1;
		j = i2;
		v = i3;
	}
};
//estrutura auxiliar para facilitar o acesso dos vértices por grau e marcá-los como visitados
struct vertice{
	int label, grau;
	bool visitado;

	vector<int> adj;

	bool operator <(const vertice d) {
		return this->grau < d.grau;
	}
	vertice(int i, int j) {
		label = i;
		grau = j;
		visitado = false;
	}
};


void imprimir_matriz(vector<arco> G, int n) {
	vector < vector<bool>> MATRIZ(n);
	for (int i = 0; i < n; i++)
		MATRIZ[i] = vector<bool>(n, false);

	for (auto arco : G) {
		MATRIZ[arco.i][arco.j] = true;
		MATRIZ[arco.j][arco.i] = true;
	}

	cout << "_|_";
	for (int i = 0; i < n; i++)
		cout << i << "_";
	cout << "|" << endl;

	for (int i = 0; i < n; i++){
		cout << i << "| ";
		for (int j = 0; j < n; j++){
			if (MATRIZ[i][j] || (i == j))
				cout << "x ";
			else
				cout << "  ";
		}
		cout << "|"<< endl;
	}
}


//Assuma que a o grafo G é conexo e que a matriz é irredutível
vector<arco> Cuthill_Mckee(vector<arco> G, int n) {

	vector<int> Graus(n, 0);
	vector < vector<int>> adj(n);


	for (auto a: G){
		Graus[a.i]++;
		Graus[a.j]++;

		adj[a.i].push_back(a.j);
		adj[a.j].push_back(a.i);
	}

	vector<vertice> Vert;
	for (int i = 0; i < n; i++) {
		vertice v = vertice(i, Graus[i]);
		v.adj = adj[i];
		Vert.push_back(v);
	}

	sort(Vert.begin(), Vert.end());

	vector<int> permutacao;

	for (int i = 0; i < n; i++){
		if (Vert[i].visitado == false) {

			permutacao.push_back(Vert[i].label);
			Vert[i].visitado = true;

			for (int j = i + 1; j < n; j++) {
				bool found = ( find(Vert[i].adj.begin(), Vert[i].adj.end(), Vert[j].label) != Vert[i].adj.end() );

				if (found && Vert[j].visitado == false) {
					permutacao.push_back(Vert[j].label);
					Vert[j].visitado = true;
				}
			}
		}
		else
			continue;
	}

	vector<int> y(n, 0);

	for (int i = 0; i < n; i++)
		y[i] = permutacao[n - 1 - i];

	vector<arco> G_new;
	for (auto a: G){
		G_new.push_back(arco(y[a.i], y[a.j], a.v));
	}

	return G_new;
}
