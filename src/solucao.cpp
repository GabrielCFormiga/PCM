#include "solucao.h"
#include <iostream>
#include <iomanip>

solucao::solucao() {

}

// inicializa o objeto com uma matriz de tamanho n + 1, m + 1
// linha 0 guarda indicacao das partes
// coluna 0 guarda indicacao das maquina
solucao::solucao(int n, int m) {
    num_maquinas = n;
    num_partes = m;
    matriz.resize(n + 1);
    for (int i = 0; i <= n; ++i) {
        matriz[i].resize(m + 1, " ");
    }

    // nomeia colunas P1, P2, ..., Pm
    for (int j = 1; j <= m; j++) {
        std::string str = "P";
        str += std::to_string(j); 
        matriz[0][j] = str;
    } 

    // nomeia linhas M1, M2, ..., Mn
    for (int i = 1; i <= n; i++) {
        std::string str = "M";
        str += std::to_string(i);
        matriz[i][0] = str;
    } 
}

solucao::~solucao(){

}

// faz a leitura dos n * m elemntos da matriz
// cria dois clusters iniciais
// calcula a quantidade de 1's na matriz
void solucao::atualizaMatriz() {
    int soma = 0;

    // faz a leitura da matriz
    for (int i = 1; i <= num_maquinas; i++) {
        for (int j = 1; j <= num_partes; j++) {
            std::cin >> matriz[i][j];
            if(matriz[i][j] == "0") matriz[i][j] = " ";
            else soma++;
        }
    }

    // cria os clusters iniciais
    if(!clusters.empty()) clusters.clear();
    num_clusters = 2;
    int imid = (1 + num_maquinas) / 2;
    int jmid = (1 + num_partes) / 2;
    clusters.push_back(cluster(1, 1, imid, jmid));
    clusters.push_back(cluster(imid + 1, jmid + 1, num_maquinas, num_partes));

    // atualiza a soma total de 1's
    num_1 = soma;
}

// troca de posicao as linhas x e y
void solucao::trocarLinhas(int x, int y) {
    for (int j = 0; j <= num_partes; j++) {
        std::string tmp =  matriz[x][j];
        matriz[x][j] = matriz[y][j];
        matriz[y][j] = tmp;
    }
}

// troca de posicao as colunas x e y
void solucao::trocarColunas(int x, int y) {
    for (int i = 0; i <= num_maquinas; i++) {
        std::string tmp =  matriz[i][x];
        matriz[i][x] = matriz[i][y];
        matriz[i][y] = tmp;
    }
}

void solucao::exibeMatriz() {
    int posj; // para viajar pelo vetor de clusters por colunas
    int posi = 0; // para viajat pelo vetor de cluster por linhas

    for (int i = 0; i <= num_maquinas; i++) {
        posj = 0;

        for (int j = 0; j <= num_partes; j++) {
            std::cout << std::setw(5) << matriz[i][j];

            // imprime o limite vertical dos clusters
            if (clusters[posj].jbr == j && j != num_partes) {
                std::cout << std::setw(5) << "|";
                posj++;
            }

            if(j == num_partes) std::cout << std::endl;
        }

        // imprime o limite horizontal dos clusters
        if (clusters[posi].ibr == i && i != num_maquinas) {
            int len = 5 * (num_partes + clusters.size() - 1);

            std::cout << std::setw(5) << "-"; // indentação do primeiro "-"
            for (int k = 1; k <= len; k++) {
                std::cout << "-";

                if(k == len) std::cout << std::endl;
            }

            posi++;
        }
    }
}

// divide o cluster na posicao pos em dois clusters
void solucao::splitCluster(int pos) {
    int imid = (clusters[pos].itl + clusters[pos].ibr) / 2;
    int jmid = (clusters[pos].jtl + clusters[pos].jbr) / 2;
    
    cluster add(imid + 1, jmid + 1, clusters[pos].ibr, clusters[pos].jbr);
    clusters[pos].atualizaBR(imid, jmid);
    
    clusters.insert(clusters.begin() + pos + 1, add);
    num_clusters++;
}

// atualiza as informações do cluster na posição pos
// atualiza pior linha e pior coluna
void solucao::atualizaCluster(int pos) {
    int itl = clusters[pos].itl;
    int ibr = clusters[pos].ibr;
    int jtl = clusters[pos].jtl;
    int jbr = clusters[pos].jbr;
    
    // atualiza pior linha
    int maior_num_de_zeros = -1;
    int pos_pior_linha;

    for (int i = itl; i <= ibr; i++) {
        int num_de_zeros = 0;
    
        for(int j = jtl; j <= jbr; j++) {
            if (matriz[i][j] != "1") num_de_zeros++;
        }

        if (num_de_zeros > maior_num_de_zeros) {
            maior_num_de_zeros = num_de_zeros;
            pos_pior_linha = i;
        }
    }

    clusters[pos].pior_linha = pos_pior_linha;
    clusters[pos].num_zeros_pior_linha = maior_num_de_zeros;

    // atualiza pior coluna
    maior_num_de_zeros = -1;
    int pos_pior_coluna;

    for (int j = jtl; j <= jbr; j++) {
        int num_de_zeros = 0;
    
        for(int i = itl; i <= ibr; i++) {
            if (matriz[i][j] != "1") num_de_zeros++;
        }

        if (num_de_zeros > maior_num_de_zeros) {
            maior_num_de_zeros = num_de_zeros;
            pos_pior_coluna = j;
        }
    }

    clusters[pos].pior_coluna = pos_pior_coluna;
    clusters[pos].num_zeros_pior_coluna = maior_num_de_zeros;
}

// retorna a posicao do cluster com maior numero de buracos
// indiretamente atualiza:
//          - o percentual de buracos em cada cluster
//          - o numeoro de buracos em cada cluster
int solucao::procuraPiorCluster() {
    float maior_percentual = -1, pos;

    for (int k = 0; k < num_clusters; k++) {
        int soma_buracos = 0;
        
        for (int i = clusters[k].itl; i <= clusters[k].ibr; i++) {
             for (int j = clusters[k].jtl; j <= clusters[k].jbr; j++) {
                if (matriz[i][j] != "1") soma_buracos++;
            }
        }

        clusters[k].num_buracos = soma_buracos;
        int largura = clusters[k].jbr - clusters[k].jtl + 1;
        int altura = clusters[k].ibr - clusters[k].itl + 1;
        clusters[k].percentual_de_buracos = (float)(soma_buracos) / ((largura) * (altura)); // largura * altura = numero total de elementos no cluster 

        if (clusters[k].percentual_de_buracos > maior_percentual) {
            maior_percentual = clusters[k].percentual_de_buracos;
            pos = k;
        }
    }

    return pos; 
}