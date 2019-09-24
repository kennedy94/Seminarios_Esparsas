#include "Header.h"

void main() {
	vector<vector<int>> MATRIZ = { { 0, 1, 1 },{ 1, 2, 1 },{ 1, 3, 1 },{ 1, 4, 1 },{ 1, 5, 1 },{ 1, 6, 1 } };

	int n = 7,
		tau = 6;

	vector<arco> G;
	for (int i = 0; i < tau; i++)
		G.push_back(arco(MATRIZ[i][0], MATRIZ[i][1], MATRIZ[i][2]));


	imprimir_matriz(G, n, tau);

	cout << endl << endl;

	imprimir_matriz(Cuthill_Mckee(G, n, tau), n, tau);

	cout << "Hello World!" << endl;

}