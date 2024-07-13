#pragma once
#include <vector>

class solucao {
    public:
    // atributos
        std::vector<std::vector<int>> matriz;

        // clusters[i - 1].first = qtd de maquinas no cluster i
        // clusters[i - 1].second = qtd de partes no cluster i
        std::vector<std::pair<int,int>> clusters;

        // eficaciaClusters[i - 1] = eficacia do cluster i
        // eficacia = n1(cluster) / (n1(cluster) + n0(cluster))
        std::vector<float> eficaciaClusters; 

        // maquinas[i - 1] = x entao a maquina i pertence ao cluster x
        std::vector<int> maquinas; 

        // partes[i - 1] = x entao a parte i pertence ao cluster x
        std::vector<int> partes; 

        int qtdMaquinas, qtdPartes, qtdClusters;

        // n1 é a qtd de 1's total na matriz
        // n1out é a qtd de 1's fora de clusters
        // n0in é a qtd de 0's dentro de clusters
        int n1, n1out, n0in;

        // eficacia = (n1 - n1out) / (n1 + n0in)
        float eficacia;

    // construtores
        solucao();
        ~solucao();
        solucao(std::vector<std::vector<int>> &vec);
        solucao(int qtdM, int qtdP);

    // métodos
        void setMatriz();

        // geração de vizinhanças de forma aleatória
        int unionCluster();
        int splitCluster();
        int moverMaquina();
        int moverParte();
        int swapMaquinas();
        int swapPartes();

        // geração eficiente de vizinhanças
        int unionPiorCluster();
        
        int moverPior();
        int moverPiorMaquina2();
        int moverPiorParte2();
        int moverPiorMaquina();
        int moverPiorParte();

        int swapPioresMaquinas();
        int swapPioresPartes();

        // métodos de perturbação
        void perturbaMaquinas();
        void perturbaPartes();
        
        // outros métodos
        void atualizaEficaciaClusters();
        float getFObj();
        void exibeSolucao();
};
