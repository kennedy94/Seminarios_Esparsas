#include "Header.h"

void main() {
	//vector<vector<int>> MATRIZ = { { 0, 1, 1 },{ 1, 2, 1 },{ 1, 3, 1 },{ 1, 4, 1 },{ 1, 5, 1 },{ 1, 6, 1 } };

	int n = 15;
	vector<arco> G = ler_instancia("matrix_exemplo_15.txt");


	imprimir_matriz(G, n);

	cout << endl << endl;

	imprimir_matriz(Cuthill_Mckee(G, n), n);

	cout << "Hello World!" << endl;

}