#include "solucao.h"
#include <iostream>
#include <algorithm>

solucao::solucao(){

}

// inicializa o objeto de acordo com as dimensões da matriz de entrada
solucao::solucao(int qtdM, int qtdP){
    qtdMaquinas = qtdM;
    qtdPartes = qtdP;

    matriz.resize(qtdM);
    for (int i = 0; i < qtdM; i++) {
        matriz[i].resize(qtdP);
    }

    maquinas.resize(qtdM);
    partes.resize(qtdP);

    // note que o numero maximo de clusters é min(qtdM, qtdP)
    // contudo, o vetor é usado de i = 0 até i < qtdClusters
    clusters.resize(std::min(qtdM, qtdP));
}

// faz a leitura da matriz do problema
// calcula n1
void solucao::setMatriz() {
    int soma = 0;

    for (int i = 0; i < qtdMaquinas; i++) {
        for (int j = 0; j < qtdPartes; j++) {
            std::cin >> matriz[i][j];         
            if (matriz[i][j] == 1) soma++;
        }
    }

    n1 = soma;
}

// une os clusters x e y
void solucao::unionCluster(int x, int y) {
    if (x = y) return;
    if (x < y) std::swap(x, y); 

    // une as maquinas
    for (int i = 0; i < qtdMaquinas; i++) {
        if (maquinas[i] == x) {
            maquinas[i] = y;
            clusters[x - 1].first--;
            clusters[y - 1].first++;
        }

        if (maquinas[i] > x) {
            int z = maquinas[i];
            clusters[z - 1].first--;
            maquinas[i] = z - 1;
            clusters[z - 2].first++;
        }
    }   

    // une as partes 
    for (int i = 0; i < qtdPartes; i++) {
        if (maquinas[i] == x) {
            maquinas[i] = y;
            clusters[x - 1].second--;
            clusters[y - 1].second++;
        }

        if (maquinas[i] > x) {
            int z = maquinas[i];
            clusters[z - 1].second--;
            maquinas[i] = z - 1;
            clusters[z - 2].second++;
        }
    }

    qtdClusters--;
}

// se possivel, divide o cluster x
// ?? mudar para int e return 0 se foi n possivel e 1 se foi ??
void solucao::splitCluster(int x) {
    if (clusters[x - 1].first == 1 || clusters[x - 1].second == 1) return;


}
