#include "Header.h"
#include <chrono>
void main() {

	char *arquivo = "esparsa_verdade.txt";
	ifstream instancia(arquivo, ifstream::in);
	int n, tau;
	instancia >> n >> tau;
	instancia.close();

	vector<arco> G = ler_instancia(arquivo);


	//imprimir_matriz(G, n);

	cout << endl << endl;

	auto TEMPO_COMECO = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed;
	G = Reversed_Cuthill_Mckee(G, n);
	auto  TEMPO_FIM = chrono::high_resolution_clock::now();
	elapsed = TEMPO_FIM - TEMPO_COMECO;
	cout << "Tempo em segundos: " << elapsed.count() << endl;

	//imprimir_matriz(G, n);
	imprimir_matriz_txt(G, n);

	cout << endl;

}