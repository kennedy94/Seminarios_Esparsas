#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <list>>
#include <stack>
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

	arco() {
		i = -1;
		j = -1;
		v = 0.0;
	}

};
//estrutura auxiliar para facilitar o acesso dos vértices por grau e marcá-los como visitados
struct vertice{
	int label, grau;
	bool visitado;
	int particao;
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
		particao = -1;
	}
};

void countSort(vector<vertice> &array, int max);

vector<arco> ler_instancia(const char * filename) {
	ifstream instancia(filename, ifstream::in);
	if (instancia.fail()) {
		cerr << "     Arquivo \"" << filename << "\" nao encontrado." << endl;
		exit(0);
	}
	int n, tau;
	instancia >> n >> tau;

	vector<arco> G(tau);
	for (int it = 0; it < tau; it++){
		int i, j; float v;

		instancia >> i >> j >> v;

		G[it] = arco(i, j, v);

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
vector<arco> Reversed_Cuthill_Mckee(vector<arco> G, int n) {
	//declarar vetor auxiliar de graus e conjuntos de adjacência
	vector<int> Graus(n, 0);
	vector<vector<int>> adj(n);

	//percorrer o grafo para atualizar graus e conjuntos de adjacência

	for (int i = 0; i < G.size(); i++)
	{
		Graus[G[i].i]++;
		Graus[G[i].j]++;

		adj[G[i].i].push_back(G[i].j);
		adj[G[i].j].push_back(G[i].i);
	}



	//jogar dados na estrutura de vértice
	vector<vertice> Vert(n);
	for (int i = 0; i < n; i++) {
		Vert[i] = vertice(i, Graus[i]);
		Vert[i].adj = adj[i];
		Vert[i].particao = -1;
	}

	//para cada vértice adicionar adjacentes por ordem de grau

	list<vertice> filhos;

	//Nó inicial
	vector<int> permutacao(n, -1);
	vertice &min = *min_element(Vert.begin(), Vert.end());
	min.visitado = true;
	min.particao++; // Pertence a particao S0
	int itp = 0;
	permutacao[itp] = min.label;
	itp++;

	vector<vertice> aux;

	int n_particoes;

	for (int i = 0; i < n; i++){
		//percorrer seus filhos e adicioná-los em ordem de grau caso não tenham sido visitados
		for (auto filho: Vert[permutacao[i]].adj) {
			//se for filho e ainda não foi visitado
			if (Vert[filho].visitado == false) {
				aux.push_back(Vert[filho]);
				//inserir_ordenado(aux, Vert[filho]);
				Vert[filho].visitado = true;
				Vert[filho].particao = Vert[permutacao[i]].particao + 1; //filho pertence a particao do pai +1

				n_particoes = Vert[filho].particao + 1; // guardar o número de particoes no momento;
			}
		}
		sort(aux.begin(), aux.end());
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

	//for (int i = 0; i < n; i++){
	//	Vert[i].particao = n_particoes - 1 - Vert[i].particao;
	//}

	//criando novo grafo (trocar rótulos)
	vector<int> transformar(n);
	for (int i = 0; i < n; i++)
		transformar[y[i]] = i;

	vector<arco> G_new(G.size());
	for (int i = 0; i < G.size(); i++)
		G_new[i] = arco(transformar[G[i].i], transformar[G[i].j], G[i].v);

	countSort(Vert, n_particoes);

	return G_new;
}


list<list<arco>> REFINED_QUOCIENT_TREES(vector<arco> G, int n) {

	stack<list<arco>> pilha();




	return list<list<arco>>();
}


void insertion_sort_m(vector<int> P) {
	int N = P.size();
	vector<int> B(N, 0);

	B[N-1] = 0;

	for (int k = N-2; k >= 0; k--){
		if (P[k] < P[k+1]){
			B[k] = 0;
		}
		else {
			if (P[k] == P[k + 1]) {
				B[k] = B[k + 1] + 1;
				continue;
			}
			int temp = P[k];
			
			int l = k + 1;
			
			while(true){
				P[l - 1] = P[l + B[l]];
				B[l - 1] = B[l];

				l += B[l] + 1;

				if (l > N - 1 || P[l] >= temp) {
					P[l - 1] = temp;
					if (P[l] == temp) {
						B[l - 1] = B[l] + 1;
					}
					else {
						B[l - 1] = 0;
					}
					break;
				}

				
			}

		}
	}
	cout << endl;

}

void countSort(vector<vertice> &array, int max) {
	int n = array.size();
	vector<vertice> output(n + 1);

	vector<int> count(max + 1, 0);     //create count array (max+1 number of elements)

	for (int i = 0; i < n; i++)
		count[array[i].particao]++;     //contar frequencia

	for (int i = 1; i <= max; i++)
		count[i] += count[i - 1];     //contar frequencia acumulada

	for (int i = n - 1; i >= 0; i--) {
		output[count[array[i].particao]] = array[i];
		count[array[i].particao] -= 1; //atualizar vetor e decrescer frequencia
	}
	for (int i = 0; i < n; i++) {
		array[i] = output[i + 1]; //store output array to main array
	}
}