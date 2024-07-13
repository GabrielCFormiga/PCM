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

// inicializa o objeto a partir de uma matriz
// cria os clusters iniciais
// qtdClusters = 2
// também faz o papel de setMatriz: calcula n1 e a eficacia inicial
solucao::solucao(std::vector<std::vector<int>> &vec) {
    qtdMaquinas = vec.size();
    qtdPartes = vec[0].size();

    matriz.resize(qtdMaquinas);
    for (int i = 0; i < qtdMaquinas; i++) {
        matriz[i].resize(qtdPartes);
    }

    maquinas.resize(qtdMaquinas);
    partes.resize(qtdPartes);

    clusters.resize(std::min(qtdMaquinas, qtdPartes));
    eficaciaClusters.resize(std::min(qtdMaquinas, qtdPartes));

    // gera os clusters iniciais
    qtdClusters = 2;

    for (int i = 0; i < qtdMaquinas; i++) {
        if (i < qtdMaquinas / 2) {
            // adiciona a máquina ao cluster 1
            maquinas[i] = 1;
            clusters[0].first++;
        } else {
            // adiciona a máquina ao cluster 2
            maquinas[i] = 2;
            clusters[1].first++;
        }
    }

    for (int i = 0; i < qtdPartes; i++) {
        if (i < qtdPartes / 2) {
            // adiciona a parte ao cluster 1
            partes[i] = 1;
            clusters[0].second++;
        } else {
            // adiciona a parte ao cluster 2
            partes[i] = 2;
            clusters[1].second++;
        }
    }


    // faz o papel de setMatriz()
    int soma = 0;

    for (int i = 0; i < qtdMaquinas; i++) {
        for (int j = 0; j < qtdPartes; j++) {
            matriz[i][j] = vec[i][j];         
            if (matriz[i][j] == 1) soma++;
        }
    }

    n1 = soma;
    getFObj();
}

// inicializa o objeto de acordo com as dimensões da matriz de entrada
// cria os clusters iniciais
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
    eficaciaClusters.resize(std::min(qtdM, qtdP));

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

// une o pior cluster com o cluster mais compatível
// retorna 0 se não foi possível unir dois clusters
// retorna 1 se foi possível
int solucao::unionPiorCluster() {
    if (qtdClusters == 1) return 0;

    atualizaEficaciaClusters();

    // procura o pior cluster
    int piorC;
    float minEficacia = 1.1;
    for (int c = 1; c <= qtdClusters; c++) {
        if (eficaciaClusters[c - 1] < minEficacia) {
            minEficacia = eficaciaClusters[c - 1];
            piorC = c;
        }       
    }

    std::vector<int> maquinasPiorC;
    std::vector<int> partesPiorC;
    for (int m = 1; m <= qtdMaquinas; m++) if (maquinas[m - 1] == piorC) maquinasPiorC.push_back(m);
    for (int p = 1; p <= qtdPartes; p++) if (partes[p - 1] == piorC) partesPiorC.push_back(p);

    // procura o cluster mais compatível
    // compatibilidade = n1_comp / (n1_comp + n0_comp)
    // n1 são os 1`s gerados pela união e n0 são os 0`s gerados pela união
    int melhorC;
    float maxComp = -1; 

    for (int c = 1; c <= qtdClusters; c++) {
        if (c == piorC) continue;

        int n1_comp = 0, n0_comp = 0;

        // verifica a compatibilidade das maquinas
        for (int i = 0; i < maquinasPiorC.size(); i++) {
            for (int p = 1; p <= qtdPartes; p++) {
                if (partes[p - 1] == c) {
                    if (matriz[maquinasPiorC[i] - 1][p - 1] == 1) n1_comp++;
                    else n0_comp++;
                }
            }
        }

        // verifica a compatibilidade das partes
        for (int i = 0; i < partesPiorC.size(); i++) {
            for (int m = 1; m <= qtdMaquinas; m++) {
                if (maquinas[m - 1] == c) {
                    if (matriz[m - 1][partesPiorC[i] - 1] == 1) n1_comp++;
                    else n0_comp++;
                }
            }
        }

        float comp = (float) n1_comp / (n1_comp + n0_comp);
        if (comp > maxComp) {
            maxComp = comp;
            melhorC = c;
        }
    }

    // une piorC com melhorC
    if (piorC < melhorC) std::swap(piorC, melhorC);

    // une as maquinas
    for (int i = 0; i < qtdMaquinas; i++) {
        if (maquinas[i] == piorC) {
            maquinas[i] = melhorC;
            clusters[piorC - 1].first--;
            clusters[melhorC - 1].first++;
        }

        if (maquinas[i] > piorC) {
            int z = maquinas[i];
            clusters[z - 1].first--;
            maquinas[i] = z - 1;
            clusters[z - 2].first++;
        }
    }   

    // une as partes 
    for (int i = 0; i < qtdPartes; i++) {
        if (partes[i] == piorC) {
            partes[i] = melhorC;
            clusters[piorC - 1].second--;
            clusters[melhorC - 1].second++;
        }

        if (partes[i] > piorC) {
            int z = partes[i];
            clusters[z - 1].second--;
            partes[i] = z - 1;
            clusters[z - 2].second++;
        }
    }

    qtdClusters--;

    return 1;
}

// move a pior parte ou máquina do pior cluster para o cluster mais compatível
// retorna 0 se não foi possível mover
// retorna 1 se foi possível
int solucao::moverPior() {
    if (qtdClusters == 1) return 0; // não faz sentido a operação

    atualizaEficaciaClusters();

    // procura o pior cluster que pode ter uma máquina ou uma parte movida
    int piorC = -1;
    float minEficacia = 1.1;
    for (int c = 1; c <= qtdClusters; c++) {
        if ((clusters[c - 1].first > 1 || clusters[c - 1].second > 1) && eficaciaClusters[c - 1] < minEficacia) {
            minEficacia = eficaciaClusters[c - 1];
            piorC = c;
        }       
    }

    if (piorC == -1) return 0; // não há cluster que possa ter uma máquina ou parte movida
    
    if (clusters[piorC - 1].first == 1) {
        moverPiorParte2();
        return 1;
    }
    
    if (clusters[piorC - 1].second == 1) {
        moverPiorMaquina2();
        return 1;
    }

    // seleciona a pior máquina 
    int mPior, mPiorSoma = qtdPartes + 1;

    for (int m = 1; m <= qtdMaquinas; m++) {
        if (maquinas[m - 1] == piorC) {
            int soma = 0;

            for (int p = 1; p <= qtdPartes; p++) {
                if (partes[p - 1] == piorC) {
                    soma += matriz[m - 1][p - 1];
                }
            }

            if (soma < mPiorSoma) {
                mPiorSoma = soma;
                mPior = m;
            }
        }
    }

    // seleciona a pior parte 
    int pPior, pPiorSoma = qtdMaquinas + 1;

    for (int p = 1; p <= qtdPartes; p++) {
        if (partes[p - 1] == piorC) {
            int soma = 0;

            for (int m = 1; m <= qtdMaquinas; m++) {
                if (maquinas[m - 1] == piorC) {
                    soma += matriz[m - 1][p - 1];
                }
            }

            if (soma < pPiorSoma) {
                pPiorSoma = soma;
                pPior = p;
            }
        }
    }

    // compara a pior entre elas
    if (pPiorSoma <= mPiorSoma) {
        // mover a parte

        // procura o cluster com melhor compatibilidade
        // no caso crítico esse cluster é o próptio piorC
        int melhorC, melhorSoma = -1;
        for (int c = 1; c <= qtdClusters; c++) {
            int soma = 0;
            
            for (int m = 1; m <= qtdMaquinas; m++) {
                if (maquinas[m - 1] == c) {
                    soma += matriz[m - 1][pPior - 1];
                }
            }

            if (soma > melhorSoma) {
                melhorSoma = soma;
                melhorC = c;
            }
        }
        
        // move a parte
        clusters[piorC - 1].second--;
        partes[pPior - 1] = melhorC;
        clusters[melhorC - 1].second++;
    } else {
        // mover a máquina

        // procura o cluster com melhor compatibilidade
        // no caso crítico esse cluster é o próptio piorC
        int melhorC, melhorSoma = -1;
        for (int c = 1; c <= qtdClusters; c++) {
            int soma = 0;
            
            for (int p = 1; p <= qtdPartes; p++) {
                if (partes[p - 1] == c) {
                    soma += matriz[mPior - 1][p - 1];
                }
            }

            if (soma > melhorSoma) {
                melhorSoma = soma;
                melhorC = c;
            }
        }
        
        // move a máquina
        clusters[piorC - 1].first--;
        maquinas[mPior - 1] = melhorC;
        clusters[melhorC - 1].first++;
    }

    return 1;
}

// move a pior máquina do pior cluster para o cluster mais compatível
// retorna 0 se não foi possível mover alguma máquina
// retorna 1 se foi possível 
int solucao::moverPiorMaquina2() {
    if (qtdClusters == 1) return 0; // não faz sentido a operação

    atualizaEficaciaClusters();

    // procura o pior cluster que pode ter uma máquina movida
    int piorC = -1;
    float minEficacia = 1.1;
    for (int c = 1; c <= qtdClusters; c++) {
        if (clusters[c - 1].first > 1 && eficaciaClusters[c - 1] < minEficacia) {
            minEficacia = eficaciaClusters[c - 1];
            piorC = c;
        }       
    }

    if (piorC == -1) return 0; // não há cluster que possa ter uma máquina movida

    // seleciona a pior máquina 
    int mPior, piorSoma = qtdPartes + 1;

    for (int m = 1; m <= qtdMaquinas; m++) {
        if (maquinas[m - 1] == piorC) {
            int soma = 0;

            for (int p = 1; p <= qtdPartes; p++) {
                if (partes[p - 1] == piorC) {
                    soma += matriz[m - 1][p - 1];
                }
            }

            if (soma < piorSoma) {
                piorSoma = soma;
                mPior = m;
            }
        }
    }

    // procura o cluster com melhor compatibilidade
    // no caso crítico esse cluster é o próptio piorC
    int melhorC, melhorSoma = -1;
    for (int c = 1; c <= qtdClusters; c++) {
        int soma = 0;
        
        for (int p = 1; p <= qtdPartes; p++) {
            if (partes[p - 1] == c) {
                soma += matriz[mPior - 1][p - 1];
            }
        }

        if (soma > melhorSoma) {
            melhorSoma = soma;
            melhorC = c;
        }
    }
    
    // move a máquina
    clusters[piorC - 1].first--;
    maquinas[mPior - 1] = melhorC;
    clusters[melhorC - 1].first++;

    return 1;
}

// move a pior parte do pior cluster para o cluster mais compatível
// retorna 0 se não foi possível mover alguma parte
// retorna 1 se foi possível 
int solucao::moverPiorParte2() {
    if (qtdClusters == 1) return 0; // não faz sentido a operação

    atualizaEficaciaClusters();

    // procura o pior cluster que pode ter uma parte movida
    int piorC = -1;
    float minEficacia = 1.1;
    for (int c = 1; c <= qtdClusters; c++) {
        if (clusters[c - 1].second > 1 && eficaciaClusters[c - 1] < minEficacia) {
            minEficacia = eficaciaClusters[c - 1];
            piorC = c;
        }       
    }

    if (piorC == -1) return 0; // não há cluster que possa ter uma parte movida

    // seleciona a pior parte 
    int pPior, piorSoma = qtdMaquinas + 1;

    for (int p = 1; p <= qtdPartes; p++) {
        if (partes[p - 1] == piorC) {
            int soma = 0;

            for (int m = 1; m <= qtdMaquinas; m++) {
                if (maquinas[m - 1] == piorC) {
                    soma += matriz[m - 1][p - 1];
                }
            }

            if (soma < piorSoma) {
                piorSoma = soma;
                pPior = p;
            }
        }
    }

    // procura o cluster com melhor compatibilidade
    // no caso crítico esse cluster é o próptio piorC
    int melhorC, melhorSoma = -1;
    for (int c = 1; c <= qtdClusters; c++) {
        int soma = 0;
        
        for (int m = 1; m <= qtdMaquinas; m++) {
            if (maquinas[m - 1] == c) {
                soma += matriz[m - 1][pPior - 1];
            }
        }

        if (soma > melhorSoma) {
            melhorSoma = soma;
            melhorC = c;
        }
    }
    
    // move a parte
    clusters[piorC - 1].second--;
    partes[pPior - 1] = melhorC;
    clusters[melhorC - 1].second++;

    return 1;
}

// move a pior máquina de um cluster (escolhido aleatoriamente) para outro
// retorna 0 se não foi possível mover alguma máquina
// retorna 1 se foi possível 
int solucao::moverPiorMaquina() {
    if (qtdClusters == 1) return 0; // não faz sentido a operação

    // armazena os cluster candidatos a terem uma máquina movida
    std::vector<int> candidatos;
    for (int i = 1; i <= qtdClusters; i++) {
        if (clusters[i - 1].first > 1) candidatos.push_back(i);
    }

    if (candidatos.empty()) return 0;
    
    // escolhe os clusters
    int pos1 = intervalRand(0, candidatos.size() - 1);
    int c1 = candidatos[pos1];
    int c2 = intervalRand(1, qtdClusters);
    while (c2 == c1) c2 = intervalRand(1, qtdClusters);

    // escolhe a pior máquina de c1
    // m1 é a sua pior máquina
    // soma1 é a sua soma de 1´s
    // note que a soma de 1´s é no máximo qtdPartes
    int m1, soma1 = qtdPartes + 1;

    for (int i = 1; i <= qtdMaquinas; i++) {
        // evita iterar por máquinas que não sejam de c1
        if (maquinas[i - 1] != c1) continue;

        int soma = 0;
        for (int j = 1; j <= qtdPartes; j++ ) {
            if (maquinas[i - 1] == partes[j - 1]) {
                soma += matriz[i - 1][j - 1];
            }
        }

        if (soma < soma1) {
            soma1 = soma;
            m1 = i;
        }
    }

    // move a máquina
    maquinas[m1 - 1] = c2;
    clusters[c1 - 1].first--;
    clusters[c2 - 1].first++;

    return 1;
}

// move a pior parte de um cluster (escolhido aleatoriamente) para outro
// retorna 0 se não foi possível mover alguma parte
// retorna 1 se foi possível 
int solucao::moverPiorParte() {
    if (qtdClusters == 1) return 0; // não faz sentido a operação

    // armazena os cluster candidatos a terem uma parte movida
    std::vector<int> candidatos;
    for (int i = 1; i <= qtdClusters; i++) {
        if (clusters[i - 1].second > 1) candidatos.push_back(i);
    }

    if (candidatos.empty()) return 0;
    
    // escolhe os clusters
    int pos1 = intervalRand(0, candidatos.size() - 1);
    int c1 = candidatos[pos1];
    int c2 = intervalRand(1, qtdClusters);
    while (c2 == c1) c2 = intervalRand(1, qtdClusters);

    // escolhe a pior parte de c1
    // p1 é a sua pior parte
    // soma1 é a sua soma de 1´s
    // note que a soma de 1´s é no máximo qtdMáquinas
    int p1, soma1 = qtdMaquinas + 1;

    for (int j = 1; j <= qtdPartes; j++) {
        // evita iterar por partes que não sejam de c1
        if (partes[j - 1] != c1) continue;

        int soma = 0;
        for (int i = 1; i <= qtdMaquinas; i++ ) {
            if (maquinas[i - 1] == partes[j - 1]) {
                soma += matriz[i - 1][j - 1];
            }
        }

        if (soma < soma1) {
            soma1 = soma;
            p1 = j;
        }
    }

    // move a parte
    partes[p1 - 1] = c2;
    clusters[c1 - 1].second--;
    clusters[c2 - 1].second++;

    return 1;
}

// escolhe dois clusters aleatoriamente
// troca as piores máquinas de cada cluster
// retorna 0 se só há um cluster, ou seja, swap não altera a eficácia da solução
// retorn 1 caso contrário
int solucao::swapPioresMaquinas() {
    if (qtdClusters == 1) return 0;

    // escolhe os clusters
    int c1 = intervalRand(1, qtdClusters);
    int c2 = intervalRand(1, qtdClusters);
    while (c2 == c1) c2 = intervalRand(1, qtdClusters);

    // escolhe as piores máquinas
    // mi é a pior máquina
    // somai é a sua soma de 1's
    // note que a soma de 1´s é no máximo qtdPartes
    int m1, soma1 = qtdPartes + 1; 
    int m2, soma2 = qtdPartes + 1;

    for (int i = 1; i <= qtdMaquinas; i++) {
        // evita iterar por máquinas que não sejam de c1 ou c2
        if (maquinas[i - 1] != c1 && maquinas[i - 1] != c2) continue;

        int soma = 0;
        for (int j = 1; j <= qtdPartes; j++) {
            if (maquinas[i - 1] == partes[j - 1]) {
                soma += matriz[i - 1][j - 1];
            }
        }

        if (maquinas[i - 1] == c1) {
            // máquina do cluster 1
            if (soma < soma1) {
                soma1 = soma;
                m1 = i;
            }

        } else {
            // máquina do cluster 2
            if (soma < soma2) {
                soma2 = soma;
                m2 = i;
            }
        }
    }

    // faz a troca
    maquinas[m1 - 1] = c2;
    maquinas[m2 - 1] = c1;

    return 1;
}

// escolhe dois clusters aleatoriamente
// troca as piores partes de cada cluster
// retorna 0 se só há um cluster, ou seja, swap não altera a eficácia da solução
// retorn 1 caso contrário
int solucao::swapPioresPartes() {
    if (qtdClusters == 1) return 0;

    // escolhe os clusters
    int c1 = intervalRand(1, qtdClusters);
    int c2 = intervalRand(1, qtdClusters);
    while (c2 == c1) c2 = intervalRand(1, qtdClusters);

    // escolhe as piores partes
    // pi é a pior parte
    // somai é a sua soma de 1´s
    // note que a soma de 1´s é no máximo qtdMaquinas
    int p1, soma1 = qtdMaquinas + 1;
    int p2, soma2 = qtdMaquinas + 1;

    for (int j = 1; j <= qtdPartes; j++) {
        // evita iterar por partes que não sejam de c1 ou c2
        if (partes[j - 1] != c1 && partes[j - 1] != c2) continue;

        int soma = 0;
        for (int i = 1; i <= qtdMaquinas; i++) {
            if (partes[j - 1] == maquinas[i - 1]) {
                soma += matriz[i - 1][j - 1];
            }
        }

        if (partes[j - 1] == c1) {
            // parte do cluster 1
            if (soma < soma1) {
                soma1 = soma;
                p1 = j;
            }
        } else {
            // parte do cluster 2
            if (soma < soma2) {
                soma2 = soma;
                p2 = j;
            }
        }
    }

    // faz a troca
    partes[p1 - 1] = c2;
    partes[p2 - 1] = c1;

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

// atualiza o vetor de eficacia dos clusters
void solucao::atualizaEficaciaClusters() {
    for (int c = 1; c <= qtdClusters; c++) {
        int n1_cluster = 0; // qtd de 1`s no cluster
        int n0_cluster = 0; // qtd de 0`s no cluster

        for (int m = 1; m <= qtdMaquinas; m++) {
            if (maquinas[m - 1] == c) {
                for (int p = 1; p <= qtdPartes; p++) {
                    if (partes[p - 1] == c){
                        if (matriz[m - 1][p - 1] == 1) n1_cluster++;
                        else n0_cluster++;
                    }
                }
            }
        }

        float efic = (float) n1_cluster / (n1_cluster + n0_cluster);
        eficaciaClusters[c - 1] = efic;
    }
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