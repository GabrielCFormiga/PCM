#include "solucao.h"
#include "snippets.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

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

    // note que o número máximo de clusters é min(qtdM, qtdP)
    // contudo, o vetor é usado de i = 0 até i = qtdClusters - 1
    clusters.resize(std::min(qtdM, qtdP));

    // gera os cluters iniciais
    qtdClusters = 2;

    for (int i = 0; i < qtdM; i++) {
        if (i < qtdM / 2) {
            // adiciona a máquina ao cluster 1
            maquinas[i] = 1;
            clusters[0].first++;
        } else {
            // adiciona a máquina ao cluster 2
            maquinas[i] = 2;
            clusters[1].first++;
        }
    }

    for (int i = 0; i < qtdP; i++) {
        if (i < qtdP / 2) {
            // adiciona a parte ao cluster 1
            partes[i] = 1;
            clusters[0].second++;
        } else {
            // adiciona a parte ao cluster 2
            partes[i] = 2;
            clusters[1].second++;
        }
    }
}

// faz a leitura da matriz do problema
// calcula n1
// calcula a eficacia inicial
void solucao::setMatriz() {
    int soma = 0;

    for (int i = 0; i < qtdMaquinas; i++) {
        for (int j = 0; j < qtdPartes; j++) {
            std::cin >> matriz[i][j];         
            if (matriz[i][j] == 1) soma++;
        }
    }

    n1 = soma;
    getFObj();
}

// une dois clusters escolhidos aleatoriamente
// retorna 0 se não foi possível unir dois clusters
// retorna 1 se foi possível
int solucao::unionCluster() {
    if (qtdClusters == 1) return 0;

    // escolhe os clusters 
    int x = intervalRand(1, qtdClusters);
    int y = intervalRand(1, qtdClusters);
    while (y == x) y = intervalRand(1, qtdClusters);
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
    return 1;
}

// divide um cluster (escolhido aleatoriamente) pela metade
// retona 0 se não foi possível dividir algum cluster
// retorn 1 se foi possível
int solucao::splitCluster() {
    // armazena os clusters que podem ser divididos
    std::vector<int> possivel;
    for(int i = 0; i < qtdClusters; i++) {
        if (clusters[i].first > 1 && clusters[i].second > 1) possivel.push_back(i + 1);
    }

    if (possivel.empty()) return 0;

    // escolhe o cluster a ser dividido
    // c é o número do cluster
    // novo é o número do cluster que vai surgir
    int aux = intervalRand(0, possivel.size() - 1);
    int c = possivel[aux];
    int novo = qtdClusters + 1;

    // divide as maquinas
    int splits = (clusters[c - 1].first) / 2;
    
    for (int i = 0; i < qtdMaquinas; i++) {
        if (maquinas[i] == c) {
            clusters[c - 1].first--;
            maquinas[i] = novo;
            clusters[novo - 1].first++;
            splits--;
        }

        if (splits == 0) break;
    }

    // divide as partes
    splits = (clusters[c - 1].second) / 2;

    for (int i = 0; i < qtdPartes; i++) {
        if (partes[i] == c) {
            clusters[c - 1].second--;
            partes[i] = novo;
            clusters[novo - 1].second++;
            splits--;
        }

        if (splits == 0) break;
    }

    // adiciona o novo cluster à conta
    qtdClusters++;
    return 1;
}

// move uma maquina (escolhida aleatoriamente) de cluster
// retorna 0 se não foi possível mover alguma máquina
// retorna 1 se foi possível
int solucao::moverMaquina() {
    // armazena as posicoes das maquinas que podem ser movidas
    std::vector<int> possivel;
    for (int i = 0; i < qtdMaquinas; i++) {
        if (clusters[maquinas[i] - 1].first > 1) possivel.push_back(i);
    }

    if (possivel.empty()) return 0;

    // escolhe a maquina a ser movida
    // pos é a sua posicao no vetor de maquinas
    // c é o seu cluster inicial
    int aux = intervalRand(0, possivel.size() - 1);
    int pos = possivel[aux];
    int c = maquinas[pos];
    
    // escolhe o novo cluster
    int novo = intervalRand(1, qtdClusters);
    while (novo == c) novo = intervalRand(1, qtdClusters);

    clusters[c - 1].first--;
    maquinas[pos] = novo;
    clusters[novo - 1].first++;
    return 1;
}

// move uma parte (escolhida aleatoriamente) de cluster
// retorna 0 se não foi possível mover alguma parte
// retorna 1 se foi possível
int solucao::moverParte() {
   // armazena as posicoes das partes que podem ser movidas
    std::vector<int> possivel;
    for (int i = 0; i < qtdPartes; i++) {
        if (clusters[partes[i] - 1].second > 1) possivel.push_back(i);
    }

    if (possivel.empty()) return 0;

    // escolhe a parte a ser movida
    // pos é a sua posicao no vetor de partes
    // c é o seu cluster inicial
    int aux = intervalRand(0, possivel.size() - 1);
    int pos = possivel[aux];
    int c = partes[pos];
    
    // escolhe o novo cluster
    int novo = intervalRand(1, qtdClusters);
    while (novo == c) novo = intervalRand(1, qtdClusters);

    clusters[c - 1].second--;
    partes[pos] = novo;
    clusters[novo - 1].second++;
    return 1;
}

// troca duas maquinas (escolhidas aleatoriamente) de cluster
void solucao::swapMaquinas() {
    // escolhe uma maquina aleatoriamente
    // pos é a sua posicão no vetor de maquinas
    // c é o seu cluster incial
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
    int aux = intervalRand(0, possivel.size() - 1);
    int posTroca = possivel[aux];

    maquinas[pos] = maquinas[posTroca];
    maquinas[posTroca] = c;
}

// troca duas partes (escolhidas aleatoriamente) de cluster 
void solucao::swapPartes() {
    // escolhe uma parte aleatoriamente
    // pos é a sua posicao no vetor de partes
    // c é o seu cluster inicial
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
    int aux = intervalRand(0, possivel.size() - 1);
    int posTroca = possivel[aux];

    partes[pos] = partes[posTroca];
    partes[posTroca] = c; 
}

// Calcula a função objetivo
// atualiza a eficacia
float solucao::getFObj() {
    // n1in é a qtd de 1's dentro de clusters
    // n0in é a qtd de 0's dentro de clusters
    int tmp_n1in = 0, tmp_n0in = 0;
    for (int i = 0; i < qtdMaquinas; i++) {
        for (int j = 0; j < qtdPartes; j++) {
            if (maquinas[i] == partes[j]) {
                // está dentro de um cluster
                if (matriz[i][j] == 1) tmp_n1in++;
                else tmp_n0in++;
            }
        }
    }

    n1out = n1 - tmp_n1in;
    n0in = tmp_n0in;

    eficacia = ((float)(n1 - n1out)) / (n1 + n0in);
    return eficacia;
}