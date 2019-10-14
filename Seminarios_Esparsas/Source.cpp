#include "Header.h"

void main() {

	char *arquivo = "matriz_trees.txt";
	ifstream instancia(arquivo, ifstream::in);
	int n, tau;
	instancia >> n >> tau;
	instancia.close();

	vector<arco> G = ler_instancia(arquivo);


	imprimir_matriz(G, n);

	cout << endl << endl;

	imprimir_matriz(Reversed_Cuthill_Mckee(G, n), n);


	cout << endl;

}