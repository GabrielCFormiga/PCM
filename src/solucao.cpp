#include "solucao.h"
#include "snippets.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>

solucao::solucao() {

}

solucao::~solucao() {

}

// inicializa o objeto de acordo com as dimensões da matriz de entrada
// qtdClusters = 2
solucao::solucao(int qtdM, int qtdP) {
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
    if (qtdClusters == 1) return 0; // não faz sentido a operação

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
    if (qtdClusters == 1) return 0; // não faz sentido a operação

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
// retorna 0 se só há um cluster, ou seja, swap não altera a eficacia da solucao
// rerorna 1 caso contrário
int solucao::swapMaquinas() {
    if (qtdClusters == 1) return 0;

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

    return 1;
}

// troca duas partes (escolhidas aleatoriamente) de cluster 
// retorna 0 se só há um cluster, ou seja, swap não altera a eficacia da solucao
// rerorna 1 caso contrário
int solucao::swapPartes() {
    if (qtdClusters == 1) return 0;

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

    return 1;
}

// faz uma perturbação linear de máquinas
// move uma máquina do cluster i para o cluster i + 1 até que o cluster inicial receba uma máquina do último cluster 
void solucao::perturbaMaquinas() {
    // cria um vetor com as máquinas candidas de cada cluster
    // candidatas[i - 1] guarda as máquinas do cluster i
    std::vector<std::vector<int>> candidatas(qtdClusters);

    // viaja pelas máquinas olhando seus clusters para montar o vetor de candidatas 
    for (int i = 1; i <= qtdMaquinas; i++) {
        int c = maquinas[i - 1];
        candidatas[c - 1].push_back(i);
    }

    // para cada cluster move uma de suas máquinas para o cluster seguinte
    for (int c = 1; c <= qtdClusters; c++) {
        // escolhe uma máquina aleatória desse cluster
        int pos = intervalRand(0, candidatas[c - 1].size() - 1);
        int maq = candidatas[c - 1][pos];

        if (c == qtdClusters) {
            // move a máquina do último cluster para o primeiro
            maquinas[maq - 1] = 1; 
        } else {
            // move a máquina para o cluster seguinte
            maquinas[maq - 1] = c + 1;
        }
    }

    // note que não é preciso alterar cluster.first pois cada cluster perde e ganha uma máquina
    // ou seja, cluster.first nao muda
}

// faz uma perturbação linear de partes
// move uma parte do cluster i para o cluster i + 1 até que o cluster inicial receba uma parte do último cluster 
void solucao::perturbaPartes() {
    // cria um vetor com as partes candidas de cada cluster
    // candidatas[i - 1] guarda as partes do cluster i
    std::vector<std::vector<int>> candidatas(qtdClusters);

    // viaja pelas partes olhando seus clusters para montar o vetor de candidatas 
    for (int i = 1; i <= qtdPartes; i++) {
        int c = partes[i - 1];
        candidatas[c - 1].push_back(i);
    }

    // para cada cluster move uma de suas partes para o cluster seguinte
    for (int c = 1; c <= qtdClusters; c++) {
        // escolhe uma parte aleatória desse cluster
        int pos = intervalRand(0, candidatas[c - 1].size() - 1);
        int par = candidatas[c - 1][pos];

        if (c == qtdClusters) {
            // move a parte do último cluster para o primeiro
            partes[par - 1] = 1; 
        } else {
            // move a parte para o cluster seguinte
            partes[par - 1] = c + 1;
        }
    }

    // note que não é preciso alterar cluster.second pois cada cluster perde e ganha uma parte
    // ou seja, cluster.second nao muda
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

void solucao::exibeSolucao() {
    // cria a matriz exibicao
    std::vector<std::vector<std::string>> matrizExibicao(2 + qtdMaquinas + qtdClusters);
    for (int k = 0; k < matrizExibicao.size(); k++) {
        matrizExibicao[k].resize(2 + qtdPartes + qtdClusters, " ");
    }

    // estabelece as divisões iniciais
    // |
    // |
    // |
    for (int k = 2; k < matrizExibicao.size(); k++) {
        matrizExibicao[k][1] = "|";
    }
    // -----
    for (int k = 1; k < matrizExibicao[1].size(); k++) {
        matrizExibicao[1][k] = "-";
    }
    

    // calcula as sequências de maquinas e partes de acordo com os clusters
    int posi = 2;
    int posj = 2;
    for (int k = 1; k <= qtdClusters; k++) {
        // viaja pelas maquinas
        for (int maq = 1; maq <= qtdMaquinas; maq++) {
            if (maquinas[maq - 1] == k) {
                matrizExibicao[posi][0] = std::to_string(maq);
                posi++;
            }
        }
        posi++;

        // viaja pelas parte
        for (int par = 1; par <= qtdPartes; par++) {
            if (partes[par - 1] == k) {
                matrizExibicao[0][posj] = std::to_string(par);
                posj++;
            }
        }
        posj++;
    }

    // preenche a matriz exibicao
    for (int i = 2; i < matrizExibicao.size(); i++) {
        for (int j = 1; j < matrizExibicao[i].size(); j++) {
            if (matrizExibicao[i][0] == " ") {
                matrizExibicao[i][j] = "-";
            } else if (matrizExibicao[0][j] == " ") {
                matrizExibicao[i][j] = "|";
            } else {
                int maq = std::stoi(matrizExibicao[i][0]);
                int par = std::stoi(matrizExibicao[0][j]);

                if (matriz[maq - 1][par - 1] == 1) {
                    matrizExibicao[i][j] = "1";
                } else {
                    matrizExibicao[i][j] = " ";
                }
            }
        }
    }

    // exibe
    for (int i = 0; i < matrizExibicao.size(); i++) {
        for (int j = 0; j < matrizExibicao[i].size(); j++) {
            std::cout << std::setw(4) << matrizExibicao[i][j];
        }
        std::cout << std::endl;
    }

}