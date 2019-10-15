#include "Header.h"

void main() {

	char *arquivo = "matrix_exemplo_15.txt";
	ifstream instancia(arquivo, ifstream::in);
	int n, tau;
	instancia >> n >> tau;
	instancia.close();

	vector<arco> G = ler_instancia(arquivo);


	imprimir_matriz(G, n);

	cout << endl << endl;

	imprimir_matriz(Reversed_Cuthill_Mckee(G, n), n);


	vector<int> v = { 3,1,2,2,1,3,2,2,1 };

	cout << endl;

}