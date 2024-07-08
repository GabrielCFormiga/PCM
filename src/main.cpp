#include <iostream>
#include <math.h>
#include "solucao.h"
#include "snippets.h"
#include "leitor.h"

using namespace std;

const float e = 2.7182818284590;

int main() {
    /* faz a leitura da matriz do problema manulmente
    int n, m;
    cin >> n >> m;

    solucao s(n, m);
    s.setMatriz();
    */

    // faz a leitura da matriz do problema pelo leitor
    leitor l;
    solucao s(l.matriz);

    // começo do SA
    solucao sMelhor = s;
    float alpha = 0.98; // coeficiente de resfriamento
    float T = 4000;
    int iterT = 0; // número de iterações na temperatura T 
    int SAmax = 500;
    int contador = 0;

    while (T > 0.00001) {
        //cout << "T = " << T << endl;
        while (iterT < SAmax) {
            iterT++;
            contador++;

            //cout << "\titerT = " << iterT << endl; 

            int op = intervalRand(0, 100);
            solucao sLinha = s;

            // 10% union, 10% split
            // 20% moverPiorMaquina, 20% moverPiorParte
            // 5% moverMaquina, 5% moverParte
            // 10% swapPioresMaquinas, 10% swapPioresPartes
            // 5% swapMaquina, 5% swapParte
            if (op <= 10) {
                // union
                //cout << "\tpassou union" << endl;
                if (sLinha.unionCluster() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitCluster();
                }
            } else if (op <= 20) {
                // split
                //cout << "\tpassou split" << endl;
                if (sLinha.splitCluster() == 0) {
                    // caso em que o número de clusters é máximo
                    // mover uma parte ou uma máquina é impossível
                    // se a matriz não for quadrada uma das operações de mover é possível
                    // escolha entre swapParte, swapMaquina ou unionCluster
                    int opNovo = intervalRand(0, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opNovo = intervalRand(0, 80); // caso em que uma das operações de mover é possível

                    if (opNovo <= 20) {
                        sLinha.unionCluster();
                    } else if (opNovo <= 40) {
                        sLinha.swapPartes();
                    } else if (opNovo <= 60) {
                        sLinha.swapMaquinas();
                    } else {
                        if (s.qtdPartes > s.qtdMaquinas) {
                            // o número de partes é maior do que o de máquinas
                            sLinha.moverParte();
                        } else {
                            // o número de máquinas é maior do que o de partes
                            sLinha.moverMaquina();
                        }
                    }
                }
            } else if (op <= 40) {
                // moverPiorMaquina
                if (sLinha.moverPiorMaquina() == 0) {
                    // caso em que o número de cluster é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdMaquinas == qtdPartes, então mover uma parte também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    int opNovo = intervalRand(0, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opNovo = intervalRand(0, 80); // caso em que se pode mover uma parte

                    if (opNovo <= 20 && sLinha.qtdClusters > 1) {
                        sLinha.unionCluster();
                    } else if (opNovo <= 20) {
                        sLinha.splitCluster();
                    } else if (opNovo <= 40) {
                        sLinha.swapPartes();
                    } else if (opNovo <= 60) {
                        sLinha.swapMaquinas();
                    } else {
                        sLinha.moverParte();
                    }
                }
            } else if (op <= 60) {
                // moverPiorParte
                if (sLinha.moverParte() == 0) {
                    // caso em que o número de clusters é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdPartes == qtdMaquinas, então mover uma maquina também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    int opNovo = intervalRand(0, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opNovo = intervalRand(0, 80); // caso em que se pode mover uma maquina

                    if (opNovo <= 20 && sLinha.qtdClusters > 1) {
                        sLinha.unionCluster();
                    } else if (opNovo <= 20) {
                        sLinha.splitCluster();
                    } else if (opNovo <= 40) {
                        sLinha.swapPartes();
                    } else if (opNovo <= 60) {
                        sLinha.swapMaquinas();
                    } else {
                        sLinha.moverMaquina();
                    }
                }
            } else if (op <= 65) {
                // moverMaquina
                //cout << "\tpassou mover Maquina" << endl;
                if (sLinha.moverMaquina() == 0) {
                    // caso em que o número de cluster é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdMaquinas == qtdPartes, então mover uma parte também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    int opNovo = intervalRand(0, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opNovo = intervalRand(0, 80); // caso em que se pode mover uma parte

                    if (opNovo <= 20 && sLinha.qtdClusters > 1) {
                        sLinha.unionCluster();
                    } else if (opNovo <= 20) {
                        sLinha.splitCluster();
                    } else if (opNovo <= 40) {
                        sLinha.swapPartes();
                    } else if (opNovo <= 60) {
                        sLinha.swapMaquinas();
                    } else {
                        sLinha.moverParte();
                    }
                }
            } else if (op <= 70) {
                // moverParte
                //cout << "\tpassou mover Parte" << endl;
                if (sLinha.moverParte() == 0) {
                    // caso em que o número de clusters é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdPartes == qtdMaquinas, então mover uma maquina também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    int opNovo = intervalRand(0, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opNovo = intervalRand(0, 80); // caso em que se pode mover uma maquina

                    if (opNovo <= 20 && sLinha.qtdClusters > 1) {
                        sLinha.unionCluster();
                    } else if (opNovo <= 20) {
                        sLinha.splitCluster();
                    } else if (opNovo <= 40) {
                        sLinha.swapPartes();
                    } else if (opNovo <= 60) {
                        sLinha.swapMaquinas();
                    } else {
                        sLinha.moverMaquina();
                    }
                }
            } else if (op <= 80) {
                // swapPioresMaquinas
                if (sLinha.swapPioresMaquinas() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitCluster();
                }
            } else if (op <= 90) {
                // swapPioresPartes
                if (sLinha.swapPioresPartes() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitCluster();
                }
            } else if (op <= 95) {
                // swapMaquinas
                //cout << "\tpassou swap Maquinas" << endl;
                if (sLinha.swapMaquinas() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitCluster();
                }
            } else {
                // swapParte
                //cout << "\tpassou swap partes" << endl;
                if (sLinha.swapPartes() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitCluster();
                }
            }

            float delta = sLinha.getFObj() - s.eficacia;
            

            if (contador > 2 * SAmax) {
                contador = 0;
                T = T * 1.15;

                // perturbação
                if (s.qtdClusters < min(s.qtdMaquinas, s.qtdPartes)) {
                    s.splitCluster();
                    s.unionCluster();
                } else {
                    s.unionCluster();
                    s.splitCluster();
                }
                
                for (int i = 0; i < 4; i++) {
                    int aux = intervalRand(0, 1);

                    if (aux == 0) s.perturbaMaquinas();
                    else s.perturbaPartes();
                }
            } else if (delta > 0) {
                contador = 0;

                s = sLinha;
                if (s.eficacia > sMelhor.eficacia) {
                    sMelhor = s;
                }
            } else {
                float x = randomFloat();
                float aux = powf(e, -delta / T);
                if (x < aux) {
                    s = sLinha;
                }
            }
        }
        
        T = alpha * T;
        iterT = 0;
    }

    cout << l.identificador << endl;
    cout << "eficacia = " << sMelhor.eficacia << endl;
    cout << "n1 = " << sMelhor.n1 << endl;
    cout << "n1out = " << sMelhor.n1out << endl;
    cout << "n0in = " << sMelhor.n0in << endl;
    sMelhor.exibeSolucao();
}
