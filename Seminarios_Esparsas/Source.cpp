#include "Header.h"
#include <chrono>
int main(int argc, char *argv[]) {

	const char *arquivo = argv[1];
	ifstream instancia(arquivo, ifstream::in);
	int n, tau;
	instancia >> n >> tau;
	instancia.close();

	//vector<coordenada> G = ler_instancia(arquivo);
	vector<coordenada> G1, G2;
	vector<coordenada> G = ler_instancia_formato_dif(arquivo);

	cout << "Instancia lida!" << endl;
	cout << "Matriz Original:" << endl;
	
	
	
	float soma_tempo = 0.0;
	for (int i = 0; i < 1; i++){
		auto TEMPO_COMECO = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed;
		//G = transformar_grafo(ORDENACAO_RQV(G, n), G);
		G1 = transformar_grafo(Reversed_Cuthill_Mckee(G, n), G);
		//G = transformar_grafo(ONE_WAY_DISSECTION(G, n), G);
		//G = transformar_grafo(NESTED_DISSECTION(G, n), G);

		G2 = transformar_grafo(Sloan(G, n), G);

		//Lanczos(Laplacian_Matrix(G, n), n, 2);


		auto  TEMPO_FIM = chrono::high_resolution_clock::now();
		elapsed = TEMPO_FIM - TEMPO_COMECO;
		soma_tempo += elapsed.count();
		
	}
	cout << "Média de tempo em segundos: " << soma_tempo/1 << endl;
	
	
	//cout << "\n\nMatriz Original:\n";
	//imprimir_matriz(G, n);
	//cout << endl;
	//cout << "\n\nMatriz RCM:\n";
	//imprimir_matriz(G1, n);
	//cout << "\n\nMatriz Sloan:\n";
	//imprimir_matriz(G2, n);


	cout << "Cuthill McKee = " << calcular_banda(G1, n) << endl;

	cout << "Sloan = " << calcular_banda(G2, n) << endl;

	imprimir_matriz_txt(G, n, "ex.dat");
	imprimir_matriz_txt(G1, n, "ex_rcm.dat");
	imprimir_matriz_txt(G2, n, "ex_sloan.dat");
	cout << "env: " << envelope(G, n) << endl;
	cout << "env: " << envelope(G1, n) << endl;
	cout << "env: " << envelope(G2, n) << endl;



	/*vector<int> v = { 3,1,2,2,1,3,2,2 };
	insertion_sort_m(v);*/

	return 0;
}