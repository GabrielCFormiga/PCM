#pragma once
#include <vector>

class solucao {
// atributos
    std::vector<std::vector<int>> matriz;

    // clusters[i - 1].first = qtd de maquinas no cluster i
    // clusters[i - 1].second = qtd de partes no cluster i
    std::vector<std::pair<int,int>> clusters; 

    // maquinas[i - 1] = x entao a maquina i pertence ao cluster x
    std::vector<int> maquinas; 

    // partes[i - 1] = x entao a parte i pertence ao cluster x
    std::vector<int> partes; 

    int qtdMaquinas, qtdPartes, qtdClusters;

    // n1 é a qtd de 1's total na matriz
    int n1, n1out, n0in;

// construtores
    solucao();
    solucao(int qtdM, int qtdP);

// métodos
    void setMatriz();
    void unionCluster(int x, int y);
    int splitCluster(int x);
    int moverMaquina(int m, int c);
    int moverParte(int p, int c);
    void swapMaquinas();
    void swapPartes();
    //getFObj()


};
