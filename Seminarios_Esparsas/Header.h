#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <list>
#include <stack>
using namespace std;
struct vertice;	//estrutura auxiliar para facilitar o acesso dos v�rtices por grau e marc�-los como visitados
struct arco;	//estrutura para guardar um arco de um grafo
void countSort(vector<vertice> &array, int max);
list<list<vertice>> REFINED_QUOCIENT_TREES(vector<arco> G, int n);
list<vertice> SPAN(vector<vertice> Vertices, int v_label);
vector<int> countSort_grau(vector<int> graus);

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

	vector<arco> G(tau);
	for (int it = 0; it < tau; it++){
		int i, j; float v;

		instancia >> i >> j >> v;
		if(i != j)
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

//Assuma que a o grafo G � conexo e que a matriz � irredut�vel
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

	//declarar vetor auxiliar de graus e conjuntos de adjac�ncia
	vector<int> Graus(n, 0);
	vector<vector<int>> adj(n);

	//percorrer o grafo para atualizar graus e conjuntos de adjac�ncia

	for (int i = 0; i < G.size(); i++)
	{
		Graus[G[i].i]++;
		Graus[G[i].j]++;

		adj[G[i].i].push_back(G[i].j);
		adj[G[i].j].push_back(G[i].i);
	}
	//vector<int> ordem_por_grau = countSort_grau(Graus); //O(n + (n - 1)) = O(n) 


	//jogar dados na estrutura de v�rtice
	vector<vertice> Vert(n);
	for (int i = 0; i < n; i++) {
		Vert[i] = vertice(i, Graus[i]);
		Vert[i].adj = adj[i];
		Vert[i].particao = -1;
	}	

	//para cada v�rtice adicionar adjacentes por ordem de grau

	list<vertice> filhos;

	//N� inicial
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
		//Caso desconexo, se h� v�rtice n�o visitado pegue o n�o visitado com menor grau
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
		//percorrer seus filhos e adicion�-los em ordem de grau caso n�o tenham sido visitados
		for (auto filho: Vert[permutacao[i]].adj) {
			//se for filho e ainda n�o foi visitado
			if (Vert[filho].visitado == false) {
				aux.push_back(Vert[filho]);
				Vert[filho].visitado = true;
				Vert[filho].particao = Vert[permutacao[i]].particao + 1; //filho pertence a particao do pai + 1
				n_particoes = Vert[filho].particao + 1; // guardar o n�mero de particoes no momento;
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

	//N� inicial
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
			int minimo_i = 0;
			for (int i2 = 1; i2 < n; i2++) {
				if (!Vert[i2].visitado && Vert[i2] < Vert[minimo_i])
					minimo_i = i2;
			}

			Vert[minimo_i].visitado = true;
			itp++;
			permutacao[i] = Vert[minimo_i].label;
		}
		//percorrer seus filhos e adicion�-los em ordem de grau caso n�o tenham sido visitados
		int f;
		for (int v = 0; v < n; v++) {
			if (Vert[v].label == permutacao[i]) {
				f = v; break;
			}
		}


		for (auto filho : Vert[f].adj) {
			//se for filho e ainda n�o foi visitado e faz parte do conjunto
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
	ofstream saida("matriz_pos_CM.txt");

	saida << n << " " << G.size() << endl;
	for (auto a : G) 
		saida << a.i-1 << " " << a.j-1 << " " << 1 << endl;
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

list<list<vertice>> estrutura_de_nivel(vector<vertice> Vertices, int r) {
	int n = Vertices.size();
	list<list<vertice>> P;
	for (auto &v : Vertices)
		v.visitado = false;


	int particao = 0,
		ni = 0;
	list<vertice> temp, temp2;
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
	list<list<vertice>> P, Componentes;
	//Gerar estrutura de n�vel enraizada em r
	P = estrutura_de_nivel(Vertices, r);
	int no_p = r, p_size = P.size();

	

	//Step 3 Achar componentes no �ltimo n�vel
	while (true) {

		int ni = 0;
		int p = 0;
		for (auto l : P) {
			for (auto v : l) {
				Vertices[v.label].particao = p;
			}
			p++;
		}
		list<vertice> temp;
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

list<list<vertice>> REFINED_QUOCIENT_TREES(vector<arco> G, int n) {
	vector<int> Graus(n, 0);
	vector<vector<int>> adj(n);

	//percorrer o grafo para atualizar graus e conjuntos de adjac�ncia
	for (int i = 0; i < G.size(); i++) {
		Graus[G[i].i]++;	Graus[G[i].j]++;
		adj[G[i].i].push_back(G[i].j);		adj[G[i].j].push_back(G[i].i);
	}

	vector<vertice> Vertices(n);
	for (int i = 0; i < n; i++) {
		Vertices[i] = vertice(i, Graus[i]);
		Vertices[i].adj = adj[i];
		Vertices[i].particao = -1;
	}

Step_0:
	stack<vertice> pilha;
	list<list<vertice>> Particao, Particao_final;

	Particao = estrutura_de_nivel(Vertices, 0);
	int p = 0;
	for (auto l : Particao) {
		for (auto v : l) {
			Vertices[v.label].particao = p;
		}
		p++;
	}


	int k = Particao.size() - 1; //maior indice da particao

								 //transformar estrutura de n�veis em uma lista de listas de v�rtices
	list<vertice> temp;
	temp = Particao.back();
	vertice y = temp.back();
	list<vertice> S;
	S.push_back(y);

	vector<bool> selecionado(Vertices.size(), false);

	selecionado[y.label] = true;


	while (k >= 0) {
		//Step 1
		while (pilha.size() != 0 && pilha.top().particao == k && !selecionado[pilha.top().label]) {
			S.push_back(pilha.top());
			pilha.pop();
		}


		//Step 2
		list<vertice> Y;
		queue<vertice> fila;
		vector<bool> visitado_aux(Vertices.size(), false);
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

		while (fila.size() != 0) {
			vertice aux = fila.front();
			Y.push_back(aux);
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
		for (auto &aux_it : visitado_aux)
			aux_it = false;
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
			Particao_final.push_back(Y);
			S.clear();
			//Step 4
			for (auto ver : Y) {
				selecionado[ver.label] = true;
				for (auto adj : ver.adj) {
					if (Vertices[adj].particao == k - 1 & !selecionado[adj]) {
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

			//tra�ar caminho
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

	return Particao_final;
}

vector<int> ORDENACAO_RQV(vector<arco> G, int n) {
	vector<int> Graus(n, 0);
	vector<vector<int>> adj(n);

	//percorrer o grafo para atualizar graus e conjuntos de adjac�ncia
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
	list<list<vertice>> Particao, Particao_final;

	Particao = estrutura_de_nivel(Vertices, vp);
	int p = 0;
	for (auto l: Particao){
		for (auto v:l){
			Vertices[v.label].particao = p;
		}
		p++;
	}


	int k = Particao.size() - 1; //maior indice da particao

	//transformar estrutura de n�veis em uma lista de listas de v�rtices
	list<vertice> temp;
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
		list<vertice> Y;
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

			//coloca no conjunto sem S s� dps q percorreu todo o S na fila
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
		for (auto &aux_it : visitado_aux)
			aux_it = false;
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

			vector<int> temp = Reversed_Cuthill_Mckee_Modificado(GY_S);

			//Numera GY_S com RCM
			for (auto tempi : temp) {
				permutacao[i_per] = tempi;
				i_per++;
			}
			for (auto s : S) {
				permutacao[i_per] = s.label;
				i_per++;
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

			//tra�ar caminho
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

	return permutacao;
}
