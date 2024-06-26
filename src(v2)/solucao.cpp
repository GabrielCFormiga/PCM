#include "solucao.h"
#include "snippets.h"
#include <iostream>
#include <algorithm>

solucao::solucao(){

}

// inicializa o objeto de acordo com as dimensões da matriz de entrada
// qtdClusters = 2
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

    // gera os cluters iniciais
    qtdClusters = 2;

    for (int i = 0; i < qtdM; i++) {
        if (i < qtdM / 2) {
            maquinas[i] = 1;
            clusters[0].first++;
        } else {
            maquinas[i] = 2;
            clusters[0].first++;
        }
    }

    for (int i = 0; i < qtdP; i++) {
        if (i < qtdP / 2) {
            partes[i] = 1;
            clusters[1].second++;
        } else {
            partes[i] = 2;
            clusters[1].second++;
        }
    }
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
        if (partes[i] == x) {
            partes[i] = y;
            clusters[x - 1].second--;
            clusters[y - 1].second++;
        }

        if (partes[i] > x) {
            int z = partes[i];
            clusters[z - 1].second--;
            partes[i] = z - 1;
            clusters[z - 2].second++;
        }
    }

    qtdClusters--;
}

// se possivel, divide o cluster x pela metade e adiciona o novo cluster ao final da fila
int solucao::splitCluster(int x) {
    if (clusters[x - 1].first == 1 || clusters[x - 1].second == 1) return 0;
    
    // divide as maquinas
    int splits = (clusters[x - 1].first) / 2;
    
    for (int i = 0; i < qtdMaquinas; i++) {
        if (maquinas[i] == x) {
            clusters[x - 1].first--;
            maquinas[i] = qtdClusters;
            clusters[qtdClusters - 1].first++;
            splits--;
        }

        if (splits == 0) break;
    }

    // divide as partes
    splits = (clusters[x - 1].second) / 2;

    for (int i = 0; i < qtdPartes; i++) {
        if (partes[i] == x) {
            clusters[x - 1].second--;
            partes[i] = qtdClusters;
            clusters[qtdClusters - 1].second++;
            splits--;
        }

        if (splits == 0) break;
    }

    qtdClusters++;
    return 1;
}

// altera o cluster da maquina m para c
int solucao::moverMaquina(int m, int c) {
    if (clusters[maquinas[m - 1] - 1].first == 1) return 0;

    clusters[maquinas[m - 1] - 1].first--;
    maquinas[m - 1] = c;
    clusters[c - 1].first++;
    return 1;
}

// altera o cluster da parte p para c
int solucao::moverParte(int p, int c) {
    if (clusters[partes[p - 1] - 1].second == 1) return 0;

    clusters[partes[p - 1] - 1].second--;
    partes[p - 1] = c;
    clusters[c - 1].second++;
    return 1;
}

// troca duas maquinas escolhidas aleatoriamente de cluster
void solucao::swapMaquinas() {
    // escolhe uma maquina aleatoriamente
    // pos é a sua posicão no vetor de maquinas
    int pos = intervalRand(0, qtdMaquinas - 1);
    int c = maquinas[pos];

    // armazena as posicoes das maquinas que podem ser trocadas
    std::vector<int> possivel;
    for (int i = 0; i < qtdMaquinas; i++) {
        if (maquinas[i] != c) {
            possivel.push_back(i);
        }
    }

    // escolhe a posicao da troca aleatoriamente
    int troca = intervalRand(0, possivel.size() - 1);

    maquinas[pos] = maquinas[troca];
    maquinas[troca] = c;
}

// troca duas partes escolhidas aleatoriamente de cluster 
void solucao::swapPartes() {
    // escolhe uma parte aleatoriamente
    // pos é a sua posicao no vetor de partes
    int pos = intervalRand(0, qtdPartes - 1);
    int c = partes[pos];

    // armazena as posicaoes das partes que podem ser trocadas
    std::vector<int> possivel;
    for (int i = 0; i < qtdPartes; i++) {
        if (partes[i] != c) {
            possivel.push_back(i);
        }
    }

    // escolhe a posicao de troca aleatoriamente
    int troca = intervalRand(0, possivel.size() - 1);

    partes[pos] = partes[troca];
    partes[troca] = c; 
}