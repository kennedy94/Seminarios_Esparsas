#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <list>
#include <stack>
#include <climits>
using namespace std;
struct vertice;	//estrutura auxiliar para facilitar o acesso dos vértices por grau e marcá-los como visitados
struct arco;	//estrutura para guardar um arco de um grafo
void countSort(vector<vertice> &array, int max);
list<list<vertice>> REFINED_QUOCIENT_TREES(vector<arco> G, int n);
list<vertice> SPAN(vector<vertice> Vertices, int v_label);
vector<int> countSort_grau(vector<int> graus);

vector<vertice> minimal_separator(vector<int> niveis, vector<vertice> Li);

vector<vector<vertice>> connected_components(vector<bool> in_T, vector<vertice> V);

struct vertice {
	int label, grau;
	bool visitado;
	int particao;
	vector<int> adj;
	vector<int> ordem;

	bool operator <(const vertice d) {
		return this->grau < d.grau;
	}

	bool operator >=(const vertice d) {
		return this->grau >= d.grau;
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

void insertion_sort_m(vector<vertice> &P);

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
		if (i != j)
			G.push_back(arco(i, j, v));
	}
	instancia.close();

	return G;
}

vector<arco> ler_instancia_formato_dif(const char * filename) {
	ifstream instancia(filename, ifstream::in);
	if (instancia.fail()) {
		cerr << "     Arquivo \"" << filename << "\" nao encontrado." << endl;
		exit(0);
	}
	int n, tau;
	instancia >> n >> tau;

	vector<arco> G;
	for (int it = 0; it < tau; it++) {
		int i, j, v;

		instancia >> i >> j >> v;
		if (i != j)
			G.push_back(arco(i-1, j-1, v));
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

vector<arco> transformar_grafo(vector<int> permutacao, vector<arco> G) {
	int n = permutacao.size();
	vector<int> transformar(n);
	for (int i = 0; i < n; i++)
		transformar[permutacao[i]] = i;

	vector<arco> G_new(G.size());
	for (int i = 0; i < G.size(); i++)
		G_new[i] = arco(transformar[G[i].i], transformar[G[i].j], G[i].v);

	return G_new;
}

vector<int> Reversed_Cuthill_Mckee(vector<arco> G, int n) {

	if (G.size() == 0)
		return vector<int>(n, -1);

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
	//vector<int> ordem_por_grau = countSort_grau(Graus); //O(n + (n - 1)) = O(n) 


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
		//Caso desconexo, se há vértice não visitado pegue o não visitado com menor grau
		if (permutacao[i] == -1) {
			int minimo_i = 0;
			for (int i2 = 1; i2 < n; i2++) {
				if (!Vert[i2].visitado && Vert[i2] < Vert[minimo_i])
					minimo_i = i2;
			}

			Vert[minimo_i].visitado = true;
			Vert[minimo_i].particao = Vert[permutacao[i - 1]].particao + 1;
			itp++;
			n_particoes = Vert[minimo_i].particao + 1;
			permutacao[i] = Vert[minimo_i].label;
		}
		//percorrer seus filhos e adicioná-los em ordem de grau caso não tenham sido visitados
		for (auto filho: Vert[permutacao[i]].adj) {
			//se for filho e ainda não foi visitado
			if (Vert[filho].visitado == false) {
				aux.push_back(Vert[filho]);
				Vert[filho].visitado = true;
				Vert[filho].particao = Vert[permutacao[i]].particao + 1; //filho pertence a particao do pai + 1
				n_particoes = Vert[filho].particao + 1; // guardar o número de particoes no momento;
			}
		}
		insertion_sort_m(aux);
		//sort(aux.begin(), aux.end());
		for (auto a : aux) {
			permutacao[itp] = a.label;
			itp++;
		}
		aux.clear();
	}

	//invertendo a ordem
	vector<int> y(n, -1);
	for (int i = 0; i < n; i++) {
		y[i] = permutacao[n - 1 - i];
		//y[i] = permutacao[i];
	}

	return y;
}


vector<int> Reversed_Cuthill_Mckee_Modificado(vector<vertice> Vert) {
	int n = Vert.size();
	if (n == 0)
		return vector<int>();

	//Nó inicial
	vector<int> permutacao(n, -1);
	vertice &min = *min_element(Vert.begin(), Vert.end());
	min.visitado = true;
	min.particao++; // Pertence a particao S0
	int itp = 0;
	permutacao[itp] = min.label;
	itp++;

	vector<vertice> aux;

	//apenas caso conexo
	for (int i = 0; i < n; i++) {
		if (permutacao[i] == -1) {
			int minimo_i = 0, min = INT_MAX;
			for (int i2 = 1; i2 < n; i2++) {
				if (!Vert[i2].visitado && Vert[i2].grau < min)
					minimo_i = i2;
			}

			Vert[minimo_i].visitado = true;
			itp++;
			permutacao[i] = Vert[minimo_i].label;
		}
		//percorrer seus filhos e adicioná-los em ordem de grau caso não tenham sido visitados
		int f;
		for (int v = 0; v < n; v++) {
			if (Vert[v].label == permutacao[i]) {
				f = v; break;
			}
		}


		for (auto filho : Vert[f].adj) {
			//se for filho e ainda não foi visitado e faz parte do conjunto
			int filho_i;
			bool in_Vert = false;
			for (int v = 0; v < n; v++) {
				if (Vert[v].label == filho) {
					filho_i = v; in_Vert = true; break;
				}
			}
			if (in_Vert) {
				if (!Vert[filho_i].visitado) {
					aux.push_back(Vert[filho_i]);
					Vert[filho_i].visitado = true;
				}
			}
		}
		insertion_sort_m(aux);
		for (auto a : aux) {
			permutacao[itp] = a.label;
			itp++;
		}
		aux.clear();
	}

	//invertendo a ordem
	vector<int> y(n, -1);
	for (int i = 0; i < n; i++) {
		y[i] = permutacao[n - 1 - i];
	}

	return y;
}

void insertion_sort_m(vector<vertice> &P) {
	if (P.size() == 0)
		return;

	int N = P.size();
	P.push_back(vertice());
	
	vector<int> B(P.size(), 0);

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
			else {
				vertice temp = P[k];

				int l = k + 1;

				while(true) {
		
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
	}

	P.pop_back();
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

vector<int> countSort_grau(vector<int> graus) {
	
	int n = graus.size();
	int max = n - 1;
	vector<int> output(n + 1);

	vector<int> count(max + 1, 0);     //create count array (max+1 number of elements)

	for (int i = 0; i < n; i++)
		count[graus[i]]++;     //contar frequencia

	for (int i = 1; i <= max; i++)
		count[i] += count[i - 1];     //contar frequencia acumulada

	for (int i = n - 1; i >= 0; i--) {
		output[count[graus[i]]] = i;
		count[graus[i]] -= 1; //atualizar vetor e decrescer frequencia
	}
	vector<int> saida(n);
	for (int i = 0; i < n; i++){
		saida[i] = output[i + 1];
	}
	
	return saida;
}

list<vertice> SPAN(vector<vertice> Vertices, int v_label) {
	list<vertice> Y;
	vector<bool> visitados(Vertices.size(), false);

	stack<vertice> pilha;
	visitados[v_label] = true;

	for (auto adj : Vertices[v_label].adj) {
		pilha.push(Vertices[adj]);
		visitados[Vertices[adj].label] = true;
	}

	while (pilha.size() != 0) {
		int v = pilha.top().label;
		Y.push_back(pilha.top());
		pilha.pop();
		for (auto adj : Vertices[v].adj){
			if (!visitados[Vertices[adj].label]) {
				pilha.push(Vertices[adj]);
				visitados[Vertices[adj].label] = true;
			}
		}
	}
	return Y;
}

void imprimir_matriz_txt(vector<arco> G, int n) {
	ofstream saida("matriz_matlab_trees.txt");

	//saida << n << " " << G.size() << endl;
	for (int i = 0; i < n; i++)	{
		saida <<i + 1 << " " << i + 1 << " " << 1 << endl;
	}


	for (auto a : G) {
		saida << a.i + 1 << " " << a.j + 1 << " " << 1 << endl;
		saida << a.j + 1 << " " << a.i + 1 << " " << 1 << endl;
	}
	//for (int i = 1; i <= n; i++)
	//	saida << i << " " << i << " " << 1 << endl;

	saida.close();
}

void insertion_sort_m(vector<int> &P) {
	if (P.size() == 0)
		return;

	int N = P.size();
	P.push_back(-1);

	vector<int> B(P.size(), 0);

	B[N - 1] = 0;

	cout << "P = ";
	for (int i = 0; i < P.size(); i++) {
		cout << P[i] << " ";
	}
	cout << endl;

	cout << "B = ";
	for (int i = 0; i < B.size(); i++) {
		cout << B[i] << " ";
	}
	cout << endl << endl;

	for (int k = N - 2; k >= 0; k--) {
		
		cout << "P = ";
		for (int i = 0; i < P.size(); i++) {
			cout << P[i] << " ";
		}
		cout << endl;

		cout << "B = ";
		for (int i = 0; i < B.size(); i++) {
			cout << B[i] << " ";
		}
		cout << endl << endl;

		if (P[k] < P[k + 1]) {
			B[k] = 0;
		}
		else {
			if (P[k] == P[k + 1]) {
				B[k] = B[k + 1] + 1;
				continue;
			}
			else {
				int temp = P[k];

				int l = k + 1;

				while (true) {
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

		
	}

	cout << "P = ";
	for (int i = 0; i < P.size(); i++) {
		cout << P[i] << " ";
	}
	cout << endl;

	cout << "B = ";
	for (int i = 0; i < B.size(); i++) {
		cout << B[i] << " ";
	}
	cout << endl << endl;
	P.pop_back();
}

vector<vector<vertice>> estrutura_de_nivel(vector<vertice> Vertices, int r) {
	int n = Vertices.size();
	vector<vector<vertice>> P;
	for (auto &v : Vertices)
		v.visitado = false;


	int particao = 0,
		ni = 0;
	vector<vertice> temp, temp2;
	Vertices[r].particao = particao;
	Vertices[r].visitado = true;
	temp.push_back(Vertices[r]);
	ni++;
	P.push_back(temp);
	
	
	while (ni < n){
		bool vazio = true;
		for (auto v: temp){
			for (auto adj: v.adj){
				if (!Vertices[adj].visitado) {
					ni++;
					temp2.push_back(Vertices[adj]);
					Vertices[adj].visitado = true;
					Vertices[adj].particao = particao;
					vazio = false;
				}
			}
		}
		if (vazio) {
			for (auto &v : Vertices) {
				if (!v.visitado) {
					temp2.push_back(v);
					v.visitado = true;
					v.particao = particao;
					ni++;
				}
			}
				
		}


		particao++;
		P.push_back(temp2);
		temp = temp2;
		temp2.clear();
		
		
	}

	return P;
}

int vertice_pseudoperiferico(vector<vertice> Vertices, int r) {
	vector<vector<vertice>> P, Componentes;
	//Gerar estrutura de nível enraizada em r
	P = estrutura_de_nivel(Vertices, r);
	int no_p = r, p_size = P.size();

	

	//Step 3 Achar componentes no último nível
	while (true) {

		int ni = 0;
		int p = 0;
		for (auto l : P) {
			for (auto v : l) {
				Vertices[v.label].particao = p;
			}
			p++;
		}
		vector<vertice> temp;
		vector<bool> visitado(Vertices.size(), false);

		for (auto v : P.back()) {
			if (!visitado[v.label]) {
				queue<vertice> fila;

				vertice no = v;
				fila.push(no);
				visitado[no.label] = true;

				while (!fila.empty()) {
					no = fila.front();
					temp.push_back(no);
					fila.pop();
					for (auto adj : no.adj) {
						if (!visitado[adj] && Vertices[adj].particao == P.size() - 1) {
							fila.push(Vertices[adj]);
							visitado[adj] = true;
						}

					}

				}
				Componentes.push_back(temp);
				temp.clear();
			}
		}

		//Step 4
		bool encontrou = false;
		for (auto c : Componentes) {
			vertice menor_grau;
			int menor = INT_MAX;
			for (auto v : c) {
				if (v.grau < menor) {
					menor = v.grau;
					menor_grau = v;
				}
			}
			P = estrutura_de_nivel(Vertices, menor_grau.label);
			if (P.size() > p_size) {
				p_size = P.size();
				no_p = menor_grau.label;
				encontrou = true;
			}
		}
		if (!encontrou)
			break;
	}


	return no_p;
}

vector<int> ORDENACAO_RQV(vector<arco> G, int n) {
	vector<int> Graus(n, 0);
	vector<vector<int>> adj(n);

	//percorrer o grafo para atualizar graus e conjuntos de adjacência
	for (int i = 0; i < G.size(); i++){
		Graus[G[i].i]++;	Graus[G[i].j]++;
		adj[G[i].i].push_back(G[i].j);		adj[G[i].j].push_back(G[i].i);
	}

	vector<vertice> Vertices(n);
	for (int i = 0; i < n; i++) {
		Vertices[i] = vertice(i, Graus[i]);
		Vertices[i].adj = adj[i];
		Vertices[i].particao = -1;
	}

	int vp = vertice_pseudoperiferico(Vertices, 0);

Step_0:
	stack<vertice> pilha;
	vector<vector<vertice>> Particao, Particao_final;

	Particao = estrutura_de_nivel(Vertices, vp);
	int p = 0;
	for (auto l: Particao){
		for (auto v:l){
			Vertices[v.label].particao = p;
		}
		p++;
	}


	vector<int> nivel(Vertices.size());
	p = 0;
	for (auto &l : Particao) {
		for (auto &v : l) {
			nivel[v.label] = p;
		}
		p++;
	}

	vector<int> Numeracao_por_Particao(Particao.size());

	Numeracao_por_Particao[0] = Vertices.size() - Particao.front().size();

	int iterador_p = 0,
		soma = Numeracao_por_Particao[0];
	for (auto p : Particao) {
		soma += p.size();
		if (iterador_p != 0 && iterador_p != Particao.size()) {
			Numeracao_por_Particao[iterador_p] = Numeracao_por_Particao[iterador_p - 1] - p.size();
		}
		iterador_p++;
	}



	int k = Particao.size() - 1; //maior indice da particao

	//transformar estrutura de níveis em uma lista de listas de vértices
	vector<vertice> temp;
	temp = Particao.back();
	vertice y = temp.back();
	list<vertice> S;
	S.push_back(y);

	vector<bool> selecionado(Vertices.size(), false);

	selecionado[y.label] = true;
	vector<int> permutacao(n, -1);
	int i_per = 0;

	while (k >= 0) {
		//Step 1
		while (pilha.size() != 0 && pilha.top().particao == k && !selecionado[pilha.top().label]) {

			bool in_S = false;
			for (auto s :S){
				if (s.label == pilha.top().label)
					in_S = true;
			}
			if(!in_S)
				S.push_back(pilha.top());
			pilha.pop();
		}


		//Step 2
		vector<vertice> Y;
		queue<vertice> fila;
		vector<bool> visitado_aux(Vertices.size(), false); //vetor auxiliar para busca que gera o span
		//Determinar Span(Y) 
		for (auto it : S) {
			if (!visitado_aux[it.label]) {
				fila.push(it);
				visitado_aux[it.label] = true;
			}
			for (auto adj : it.adj) {
				if (Vertices[adj].particao == k && !visitado_aux[Vertices[adj].label]) {
					fila.push(Vertices[adj]);
					visitado_aux[Vertices[adj].label] = true;
				}
			}
		}

		vector<vertice> GY_S;
		while (fila.size() != 0) {
			vertice aux = fila.front();
			Y.push_back(aux);

			//coloca no conjunto sem S só dps q percorreu todo o S na fila
			bool in_S = false;
			for (auto s : S) {
				if (s.label == aux.label) {
					in_S = true;
					break;
				}
			}
			if(!in_S)
				GY_S.push_back(aux);

			fila.pop();

			for (auto adj : aux.adj) {
				if (Vertices[adj].particao == k && !visitado_aux[Vertices[adj].label]) {
					fila.push(Vertices[adj]);
					visitado_aux[Vertices[adj].label] = true;
				}
			}
		}

		//Determinar Adj(Y) intersecao L_{k+1}
		list<vertice> Conj_inter;
		for (int it = 0; it < Vertices.size(); it++)
			visitado_aux[it] = false;
		for (auto it : Y) {
			for (auto adj : it.adj) {
				if (Vertices[adj].particao == k + 1 && !selecionado[Vertices[adj].label] && !visitado_aux[Vertices[adj].label]) {
					Conj_inter.push_back(Vertices[adj]);
					visitado_aux[Vertices[adj].label] = true;
				}
			}
		}

		if (Conj_inter.size() == 0) {
			//Step 3
			//Gerar grafo G(Y\S)

			//Numera com RCM
			vector<int> temp = Reversed_Cuthill_Mckee_Modificado(GY_S);
			for (auto tempi : temp) {
				permutacao[Numeracao_por_Particao[nivel[tempi]]] = tempi;
				Numeracao_por_Particao[nivel[tempi]]++;
			}

			//Numera S com ordem arbitraria
			for (auto s : S) {
				permutacao[Numeracao_por_Particao[nivel[s.label]]] = s.label;
				Numeracao_por_Particao[nivel[s.label]]++;
			}

			

			Particao_final.push_back(Y);
			S.clear();
			//Step 4
			for (auto ver : Y) {
				selecionado[ver.label] = true;
				for (auto adj : ver.adj) {
					if (Vertices[adj].particao == k - 1 && !selecionado[adj] && !visitado_aux[adj]) {
						S.push_back(Vertices[adj]);
						visitado_aux[adj] = true;
					}
				}
			}
			Y.clear();
			k--;
		}
		else {
			//Step 5

			//empilha S na pilha
			for (auto s : S) {
				pilha.push(s);
			}
			//pegar vertice qualquer adj(Y) do nivel k + 1
			vertice y_k1;
			for (auto ver : Y) {
				for (auto adj : ver.adj) {
					if (Vertices[adj].particao == k + 1 && !selecionado[adj]) {
						y_k1 = Vertices[adj];
						break;
					}
				}
			}

			//traçar caminho
			int t = 0, T = 1;
			while (k + t + 1 < Particao.size()) {
				t++;
				vertice y_k1_aux = y_k1;
				for (auto filho : y_k1_aux.adj) {
					if (Vertices[filho].particao == k + t && !visitado_aux[Vertices[filho].label] && !selecionado[Vertices[filho].label]) {
						y_k1 = Vertices[filho];
						T = t;
					}
				}
			}
			S.clear();
			S.push_back(y_k1);
			k += T;
		}

	}
	//cout << endl;
	//for (auto  perm: permutacao){
	//	cout << perm << " ";
	//}

	return permutacao;
}


vector<int> ONE_WAY_DISSECTION(vector<arco> G, int n) {
	vector<int> Graus(n, 0);
	vector<vector<int>> adj(n);

	//percorrer o grafo para atualizar graus e conjuntos de adjacência
	for (int i = 0; i < G.size(); i++) {
		Graus[G[i].i]++;	Graus[G[i].j]++;
		if (find(adj[G[i].i].begin(), adj[G[i].i].end(), G[i].j) == adj[G[i].i].end())
			adj[G[i].i].push_back(G[i].j);
		if (find(adj[G[i].j].begin(), adj[G[i].j].end(), G[i].i) == adj[G[i].j].end())
			adj[G[i].j].push_back(G[i].i);
	}

	vector<vertice> Vertices(n);
	for (int i = 0; i < n; i++) {
		Vertices[i] = vertice(i, Graus[i]);
		Vertices[i].adj = adj[i];
		Vertices[i].particao = -1;
	}

	Graus.clear();
	for (auto &i : adj) {
		i.clear();
	}
	adj.clear();

//STEP1:
	//achar nó pseudoperiférico
	int vp = vertice_pseudoperiferico(Vertices, 0);

	//CRIAR ESTRUTURA DE NÍVEL
	vector<vector<vertice>> L = estrutura_de_nivel(Vertices, vp);

	int p = 0;
	for (auto l : L) {
		for (auto v : l) {
			Vertices[v.label].particao = p;
		}
		p++;
	}

	vector<int> niveis(Vertices.size());
	p = 0;
	for (auto &l : L) {
		for (auto &v : l) {
			niveis[v.label] = p;
		}
		p++;
	}

	int l = L.size();

	float m, alpha, alphat, delta;

	int k;

	vector<vertice> X;
	vector<vector<vertice>> Y;
	vector<vertice> G_;
	vector<vector<vertice>> T;
	vector<bool> in_T(n, false);

//STEP2:
	m = (float)n / (l + 1);
	bool pula = false;
	if (m > sqrt(6 * l)){
		alphat = l* sqrt(2.0 / 3 * (m + 1)) - 1;

		do
		{
//STEP3:
			delta = l / (alphat + 1.0);
			if (delta >= 2) {
				pula = true;
				break;
			}
//STEP4:
			alphat--;
		} while (alphat > 1);
	}
	if (!pula) {
//STEP5:
		Y.push_back(Vertices);
		k = Y.size();
		alpha = 0;
	}
	else {
//STEP6:
		alpha = floor(alphat);

		for (int i = 0; i < alpha; i++) {
			int j = floor((float)(i + 1) * delta - 1);
			T.push_back(minimal_separator(niveis, L[j]));
			for (auto t : T.back()) {
				in_T[t.label] = true;
			}

		}

//STEP7:

		Y = connected_components(in_T, Vertices);
		k = Y.size();
		for (auto j : T) {
			Y.push_back(j);
		}
	}

//STEP8:
	vector<int> permutacao(n, -1);
	int it = 0;
	for (int i = 0; i < k; i++) {
		vector<int> permut = Reversed_Cuthill_Mckee_Modificado(Y[i]);
		for (int ii = 0; ii < permut.size(); ii++){
			permutacao[it] = permut[ii];
			it++;
		}
	}
	for (int j = 0; j < alpha; j++){
		for (int jj = 0; jj < Y[k+j].size(); jj++) {
			permutacao[it] = Y[k + j][jj].label;
			it++;
		}
	}

	return permutacao;
}

vector<vertice> minimal_separator(vector<int> niveis, vector<vertice> Li) {
	vector<vertice> retorno;

	vector<bool> escolhido(niveis.size(), false);

	for (auto i : Li){
		for (auto f : i.adj)
			if (niveis[f] == niveis[i.label] + 1 && !escolhido[i.label]) {
				retorno.push_back(i);
				escolhido[i.label] = true;
			}
	}

	return retorno;
}


vector<vector<vertice>> connected_components(vector<bool> in_T, vector<vertice> V) {
	vector<vector<vertice>> retorno;
	vector<vertice> auxiliar;
	vector<bool> visitado(in_T.size(), false);
	queue<vertice> fila;


	for (int i = 0; i < V.size(); i++){
		if (!visitado[i] && !in_T[i]) {

			fila.push(V[i]);
			visitado[V[i].label] = true;

			while (!fila.empty()) {
				for (auto adj : fila.front().adj) {
					if (!visitado[adj] && !in_T[adj]) {
						fila.push(V[adj]);
						visitado[V[adj].label] = true;
					}
				}
				auxiliar.push_back(fila.front());
				fila.pop();
			}
			retorno.push_back(auxiliar);
			auxiliar.clear();
		}
	}



	return retorno;
}


//NESTED DISSECTION

vector<vertice> connected_component_from_v(vector<bool> in_R, vector<vertice> V) {
	vector<vertice> retorno;
	vector<bool> visitado(V.size(), false);
	queue<vertice> fila;

	int no_inicial;
	for (int i = 0; i < in_R.size(); i++){
		if (in_R[i]) {
			no_inicial = i;
			break;
		}
	}

	fila.push(V[no_inicial]);
	visitado[no_inicial] = true;

	while (!fila.empty()) {
		for (auto adj : fila.front().adj) {
			if (!visitado[adj] && in_R[adj]) {
				fila.push(V[adj]);
				visitado[V[adj].label] = true;
			}
		}
		retorno.push_back(fila.front());
		fila.pop();
	}

	return retorno;
}

vector<vector<vertice>> estrutura_de_nivel_nested(vector<vertice> Vertices, int r, vector<bool> in_V) {
	int n = Vertices.size();
	vector<vector<vertice>> P;
	for (auto &v : Vertices)
		v.visitado = false;


	int particao = 0,
		ni = 0;
	vector<vertice> temp, temp2;
	Vertices[r].particao = particao;
	Vertices[r].visitado = true;
	temp.push_back(Vertices[r]);
	ni++;
	P.push_back(temp);


	int limite_n = accumulate(in_V.begin(), in_V.end(), 0);

	while (ni < limite_n) {
		bool vazio = true;
		for (auto v : temp) {
			for (auto adj : v.adj) {
				if (!Vertices[adj].visitado && in_V[adj]) {
					ni++;
					temp2.push_back(Vertices[adj]);
					Vertices[adj].visitado = true;
					Vertices[adj].particao = particao;
					vazio = false;
				}
			}
		}
		if (vazio) {
			for (auto &v : Vertices) {
				if (!v.visitado && in_V[v.label]) {
					temp2.push_back(v);
					v.visitado = true;
					v.particao = particao;
					ni++;
				}
			}

		}


		particao++;
		P.push_back(temp2);
		temp = temp2;
		temp2.clear();


	}

	return P;
}

int vertice_pseudoperiferico_nested(vector<vertice> Vertices, int r, vector<bool> in_V) {
	vector<vector<vertice>> P, Componentes;
	//Gerar estrutura de nível enraizada em r
	P = estrutura_de_nivel_nested(Vertices, r, in_V);
	int no_p = r, p_size = P.size();



	//Step 3 Achar componentes no último nível
	while (true) {

		int ni = 0;
		int p = 0;
		for (auto l : P) {
			for (auto v : l) {
				Vertices[v.label].particao = p;
			}
			p++;
		}
		vector<vertice> temp;
		vector<bool> visitado(Vertices.size(), false);

		for (auto v : P.back()) {
			if (!visitado[v.label]) {
				queue<vertice> fila;

				vertice no = v;
				fila.push(no);
				visitado[no.label] = true;

				while (!fila.empty()) {
					no = fila.front();
					temp.push_back(no);
					fila.pop();
					for (auto adj : no.adj) {
						if (!visitado[adj] && Vertices[adj].particao == P.size() - 1) {
							fila.push(Vertices[adj]);
							visitado[adj] = true;
						}

					}

				}
				Componentes.push_back(temp);
				temp.clear();
			}
		}

		//Step 4
		bool encontrou = false;
		for (auto c : Componentes) {
			vertice menor_grau;
			int menor = INT_MAX;
			for (auto v : c) {
				if (v.grau < menor) {
					menor = v.grau;
					menor_grau = v;
				}
			}
			P = estrutura_de_nivel_nested(Vertices, menor_grau.label, in_V);
			if (P.size() > p_size) {
				p_size = P.size();
				no_p = menor_grau.label;
				encontrou = true;
			}
		}
		if (!encontrou)
			break;
	}


	return no_p;
}



vector<int> NESTED_DISSECTION(vector<arco> G, int n) {
	vector<int> Graus(n, 0);
	vector<vector<int>> adj(n);

	//percorrer o grafo para atualizar graus e conjuntos de adjacência
	for (int i = 0; i < G.size(); i++) {
		Graus[G[i].i]++;	Graus[G[i].j]++;
		if (find(adj[G[i].i].begin(), adj[G[i].i].end(), G[i].j) == adj[G[i].i].end())
			adj[G[i].i].push_back(G[i].j);
		if (find(adj[G[i].j].begin(), adj[G[i].j].end(), G[i].i) == adj[G[i].j].end())
			adj[G[i].j].push_back(G[i].i);
	}

	vector<vertice> X(n);
	for (int i = 0; i < n; i++) {
		X[i] = vertice(i, Graus[i]);
		X[i].adj = adj[i];
		X[i].particao = -1;
	}

	vector<int> permutacao(n, -1);
	vector<vector<vertice>> L, P;
	vector<vertice> S_mk;
	vector<bool> in_R(n, true);
	
	int j;

	int N = X.size();
	int tamanho_R = accumulate(in_R.begin(), in_R.end(), 0);
	//STEP2
	while (tamanho_R > 0) {
		vector<vertice> R_mk = connected_component_from_v(in_R, X);
		vector<bool> in_R_mk(n, false);
		for (auto r : R_mk) {
			in_R_mk[r.label] = true;
		}
		vector<int> niveis(n, -1);

		int y = vertice_pseudoperiferico_nested(X, R_mk.front().label, in_R_mk);
		
		//STEP3
		L = estrutura_de_nivel_nested(X, y, in_R_mk);

		int it = 0;
		for (auto l : L) {
			for (auto x : l) {
				niveis[x.label] = it;
			}
			it++;
		}

		if (L.size() > 3) {
			j = floor((L.size() + 1) / 2.0);
			//STEP4
			S_mk.clear();
			for (auto y_ : L[j]) {
				for (auto adj : y_.adj) {
					if (niveis[adj] == j + 1) {
						S_mk.push_back(y_);
						break;
					}
				}
			}
		}
		else
		{
			//STEP3 FAIL
			S_mk = R_mk;
		}

		//STEP5
		vector<int> permut = Reversed_Cuthill_Mckee_Modificado(S_mk);
		int it_p = 0;
		for (int i = N - S_mk.size(); i < N; i++){
			permutacao[i] = permut[it_p];
			it_p++;
		}
		N -= S_mk.size();


		
		//STEP6
		for (auto s : S_mk){
			in_R[s.label] = false;
			tamanho_R--;
		}
		P.push_back(S_mk);

	}


	
	

	return permutacao;
}
