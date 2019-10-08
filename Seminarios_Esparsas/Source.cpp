#include "Header.h"

void main() {
	//vector<vector<int>> MATRIZ = { { 0, 1, 1 },{ 1, 2, 1 },{ 1, 3, 1 },{ 1, 4, 1 },{ 1, 5, 1 },{ 1, 6, 1 } };

	char *arquivo = "matrix_exemplo_10.txt";
	ifstream instancia(arquivo, ifstream::in);
	int n, tau;
	instancia >> n >> tau;
	instancia.close();

	vector<arco> G = ler_instancia(arquivo);


	imprimir_matriz(G, n);

	cout << endl << endl;

	imprimir_matriz(Cuthill_Mckee(G, n), n);

	cout << "Hello World!" << endl;

}