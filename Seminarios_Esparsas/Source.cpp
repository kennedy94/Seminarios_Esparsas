#include "Header.h"
#include <chrono>
int main(int argc, char *argv[]) {

	const char *arquivo = argv[1];
	ifstream instancia(arquivo, ifstream::in);
	int n, tau;
	instancia >> n >> tau;
	instancia.close();

	vector<arco> G = ler_instancia(arquivo);
	//vector<arco> G = ler_instancia_formato_dif(arquivo);

	cout << "Instancia lida!" << endl;
	cout << "Matriz Original:" << endl;
	imprimir_matriz(G, n);
	
	
	float soma_tempo = 0.0;
	for (int i = 0; i < 1; i++){
		auto TEMPO_COMECO = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed;
		G = transformar_grafo(ORDENACAO_RQV(G, n), G);
		//G = transformar_grafo(Reversed_Cuthill_Mckee(G, n), G);
		auto  TEMPO_FIM = chrono::high_resolution_clock::now();
		elapsed = TEMPO_FIM - TEMPO_COMECO;
		soma_tempo += elapsed.count();
		
	}
	cout << "Média de tempo em segundos: " << soma_tempo/1 << endl;
	
	cout << "\n\nMatriz Reordenada:\n";
	imprimir_matriz(G, n);

	//imprimir_matriz_txt(G, n);


	/*vector<int> v = { 3,1,2,2,1,3,2,2 };
	insertion_sort_m(v);*/

	return 0;
}