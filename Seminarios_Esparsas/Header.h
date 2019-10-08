#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <list>>

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

	bool operator ==(const vertice d) {
		return this->grau == d.grau;
	}
	vertice(int i, int j) {
		label = i;
		grau = j;
		visitado = false;
	}
	vertice() {
		label = -1;
		grau = -1;
		visitado = false;

	}
};

vector<arco> ler_instancia(const char * filename) {
	ifstream instancia(filename, ifstream::in);
	if (instancia.fail()) {
		cerr << "     Arquivo \"" << filename << "\" nao encontrado." << endl;
		exit(0);
	}
	int n, tau;
	instancia >> n >> tau;

	vector<arco> G;
	for (int it = 0; it < tau; it++){
		int i, j; float v;

		instancia >> i >> j >> v;

		G.push_back(arco(i, j, v));

	}
	instancia.close();
	return G;
}

void imprimir_matriz(vector<arco> G, int n) {
	vector < vector<bool>> MATRIZ(n);
	for (int i = 0; i < n; i++)
		MATRIZ[i] = vector<bool>(n, false);

	for (auto arco : G) {
		MATRIZ[arco.i][arco.j] = true;
		MATRIZ[arco.j][arco.i] = true;
	}

	//cout << "_|_";
	//for (int i = 0; i < n; i++)
	//	cout << i << "_";
	//cout << "|" << endl;

	for (int i = 0; i < n; i++){
		//cout << i << "| ";
		cout << "| ";
		for (int j = 0; j < n; j++){
			if (MATRIZ[i][j] || (i == j))
				cout << "x ";
			else
				cout << "  ";
		}
		cout << "|"<< endl;
	}
}

void inserir_ordenado(vector<vertice> &V, vertice v) {
	vector<vertice>::iterator pos;
	if (V.size() != 0) {
		bool encontrou = false;
		for (vector<vertice>::iterator it = V.begin(); it != V.end(); it++) {
			if (*it < v) {
				pos = it;
				break;
				encontrou = true;
			}
		}
		if (encontrou) {
			V.insert(pos, v);
			v.visitado = true;
		}
		else {
			V.insert(V.begin(), v);
		}
	}
	else {
		V.push_back(v);
	}
}

//Assuma que a o grafo G é conexo e que a matriz é irredutível
//retornando o reverso
vector<arco> Cuthill_Mckee(vector<arco> G, int n) {
	//declarar vetor auxiliar de graus e conjuntos de adjacência
	vector<int> Graus(n, 0);
	vector < vector<int>> adj(n);

	//percorrer o grafo para atualizar graus e conjuntos de adjacência
	for (auto a: G){
		Graus[a.i]++;
		Graus[a.j]++;

		adj[a.i].push_back(a.j);
		adj[a.j].push_back(a.i);
	}

	//jogar dados na estrutura de vértice
	vector<vertice> Vert(n);
	for (int i = 0; i < n; i++) {
		Vert[i] = vertice(i, Graus[i]);
		Vert[i].adj = adj[i];
	}

	//para cada vértice adicionar adjacentes por ordem de grau

	list<vertice> filhos;

	//Nó inicial
	vector<int> permutacao(n, -1);
	vertice &min = *min_element(Vert.begin(), Vert.end());
	min.visitado = true;
	int itp = 0;
	permutacao[itp] = min.label;
	itp++;

	vector<vertice> aux;

	for (int i = 0; i < n; i++){
		//percorrer seus filhos e adicioná-los em ordem de grau caso não tenham sido visitados
		for (auto filho: Vert[permutacao[i]].adj) {
			//se for filho e ainda não foi visitado
			if (Vert[filho].visitado == false) {
				inserir_ordenado(aux, Vert[filho]);
				Vert[filho].visitado = true;
			}
		}
		for (auto a : aux) {
			permutacao[itp] = a.label;
			itp++;
		}
		aux.clear();
	}

	//invertendo a ordem
	vector<int> y(n, 0);
	for (int i = 0; i < n; i++)
		y[i] = permutacao[n - 1 - i];

	//criando novo grafo

	vector<int> transformar(n);
	for (int i = 0; i < n; i++)
		transformar[y[i]] = i;

	vector<arco> G_new;
	for (auto a: G){
		G_new.push_back(arco(transformar[a.i], transformar[a.j], a.v));
	}

	return G_new;
}

