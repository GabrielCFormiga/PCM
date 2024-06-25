#pragma once
#include <vector>
#include <string>
#include "cluster.h"

class solucao {
    public:
        // atributos
        std::vector<std::vector<std::string>> matriz;
        std::vector<cluster> clusters;
        int num_maquinas, num_partes, num_clusters;
        int num_1, num_1_externos, num_0_internos;
        float eficacia;

        // construtores 
        solucao();
        solucao(int n, int m);
        ~solucao();

        // métodos
        void atualizaMatriz();
        void trocarLinhas(int x, int y);
        void trocarColunas(int x, int y);
        void exibeMatriz();
        void splitCluster(int pos);
        void unionClusters(int pos);
        void atualizaCluster(int pos);
        void atualizaEficacia();
        int procuraPiorCluster();

};
