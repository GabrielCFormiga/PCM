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
    int n1;

// construtores
    solucao();
    solucao(int qtdM, int qtdP);

// métodos
    void setMatriz();
    void unionCluster(int x, int y);
    void splitCluster(int x);

};
