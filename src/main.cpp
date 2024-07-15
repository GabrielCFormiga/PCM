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
    float T = 5000;
    int iterT = 0; // número de iterações na temperatura T 
    int SAmax = 500;
    int PAmax = 1000;
    int contador = 0;

    while (T > 0.00001) {
        //cout << "T = " << T << endl;
        while (iterT < SAmax) {
            iterT++;
            contador++;
            
            //cout << "\titerT = " << iterT << endl; 

            int op = intervalRand(1, 100);
            solucao sLinha = s;

            // 10% unionPiorCluster, 10% splitPiorCluster
            // 12% moverPior2, 4% moverPiorMaquina5, 4% moverPiorParte5, 10% moverPiorMaquina2, 10% moverPiorParte2 
            // 5% moverMaquina, 5% moverParte
            // 10% swapPioresMaquinas2, 10% swapPioresPartes2
            // 5% swapMaquina, 5% swapParte
            if (op <= 10) {
                // unionPiorCluster
                
                if (sLinha.unionPiorCluster() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitPiorCluster();
                }

            } else if (op <= 20) {
                // split
                
                if (sLinha.splitPiorCluster() == 0) {
                    // caso em que o número de clusters é máximo
                    // mover uma parte ou uma máquina é impossível
                    // se a matriz não for quadrada uma das operações de mover é possível
                    // escolha entre swapParte, swapMaquina ou unionCluster

                    int opInterna = intervalRand(1, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opInterna = intervalRand(1, 80); // caso em que uma das operações de mover é possível

                    if (opInterna <= 20) {
                        sLinha.unionPiorCluster();
                    } else if (opInterna <= 40) {
                        sLinha.swapPioresPartes2();
                    } else if (opInterna <= 60) {
                        sLinha.swapPioresMaquinas2();
                    } else {
                        if (s.qtdPartes > s.qtdMaquinas) {
                            // o número de partes é maior do que o de máquinas
                            sLinha.moverPiorParte2();
                        } else {
                            // o número de máquinas é maior do que o de partes
                            sLinha.moverPiorMaquina2();
                        }
                    }
                }

            } else if (op <= 32) {
                // moverPior2

                if (sLinha.moverPior2() == 0) {
                    // caso em que o número de clusters é máximo ou 1
                    // a função moverPior já trata o caso n != m
                    // se só há um cluster o split é obrigatório

                    int opInterna = intervalRand(1, 60);
                    
                    if (sLinha.qtdClusters == 1) {
                        sLinha.splitPiorCluster();
                    } else if (op <= 20) {
                        sLinha.unionPiorCluster();
                    } else if (op <= 40) {
                        sLinha.swapPioresMaquinas2();
                    } else if (op <= 60) {
                        sLinha.swapPioresPartes2();
                    }
                }
                
            } else if (op <= 36) {
                // moverPiorMaquina5

                if (sLinha.moverPiorMaquina5() == 0) {
                    // caso em que o número de cluster é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdMaquinas == qtdPartes, então mover uma parte também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    
                    int opInterna = intervalRand(1, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opInterna = intervalRand(1, 80); // caso em que se pode mover uma parte

                    if (sLinha.qtdClusters == 1) {
                        sLinha.splitPiorCluster();
                    } else if (opInterna <= 20) {
                        sLinha.unionPiorCluster();
                    } else if (opInterna <= 40) {
                        sLinha.swapPioresPartes2();
                    } else if (opInterna <= 60) {
                        sLinha.swapPioresMaquinas2();
                    } else {
                        sLinha.moverPiorParte2();
                    }
                }

            } else if (op <= 40) {
                // moverPiorParte5

                if (sLinha.moverPiorParte5() == 0) {
                    // caso em que o número de cluster é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdMaquinas == qtdPartes, então mover uma parte também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    
                    int opInterna = intervalRand(1, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opInterna = intervalRand(1, 80); // caso em que se pode mover uma maquina

                    if (sLinha.qtdClusters == 1) {
                        sLinha.splitPiorCluster();
                    } else if (opInterna <= 20) {
                        sLinha.unionPiorCluster();
                    } else if (opInterna <= 40) {
                        sLinha.swapPioresPartes2();
                    } else if (opInterna <= 60) {
                        sLinha.swapPioresMaquinas2();
                    } else {
                        sLinha.moverPiorMaquina2();
                    }
                }

            } else if (op <= 50) {
                // moverPiorMaquina2

                if (sLinha.moverPiorMaquina2() == 0) {
                    // caso em que o número de cluster é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdMaquinas == qtdPartes, então mover uma parte também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    
                    int opInterna = intervalRand(1, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opInterna = intervalRand(1, 80); // caso em que se pode mover uma parte

                    if (sLinha.qtdClusters == 1) {
                        sLinha.splitPiorCluster();
                    } else if (opInterna <= 20) {
                        sLinha.unionPiorCluster();
                    } else if (opInterna <= 40) {
                        sLinha.swapPioresPartes2();
                    } else if (opInterna <= 60) {
                        sLinha.swapPioresMaquinas2();
                    } else {
                        sLinha.moverPiorParte2();
                    }
                }

            } else if (op <= 60) {
                // moverPiorParte2

                if (sLinha.moverPiorParte2() == 0) {
                    // caso em que o número de clusters é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdPartes == qtdMaquinas, então mover uma maquina também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    
                    int opInterna = intervalRand(1, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opInterna = intervalRand(0, 80); // caso em que se pode mover uma maquina

                    if (sLinha.qtdClusters == 1) {
                        sLinha.splitPiorCluster();
                    } else if (opInterna <= 20) {
                        sLinha.unionPiorCluster();
                    } else if (opInterna <= 40) {
                        sLinha.swapPioresPartes2();
                    } else if (opInterna <= 60) {
                        sLinha.swapPioresMaquinas2();
                    } else {
                        sLinha.moverPiorMaquina2();
                    }
                }

            } else if (op <= 65) {
                // moverMaquina
                
                if (sLinha.moverMaquina() == 0) {
                    // caso em que o número de cluster é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdMaquinas == qtdPartes, então mover uma parte também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    
                    int opInterna = intervalRand(1, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opInterna = intervalRand(1, 80); // caso em que se pode mover uma parte

                    if (sLinha.qtdClusters == 1) {
                        sLinha.splitPiorCluster();
                    } else if (opInterna <= 20) {
                        sLinha.unionPiorCluster();
                    } else if (opInterna <= 40) {
                        sLinha.swapPioresPartes2();
                    } else if (opInterna <= 60) {
                        sLinha.swapPioresMaquinas2();
                    } else {
                        sLinha.moverPiorParte2();
                    }
                }

            } else if (op <= 70) {
                // moverParte
                
                if (sLinha.moverParte() == 0) {
                    // caso em que o número de clusters é máximo ou 1
                    // caso seja 1 a operação não alteraria a eficácia
                    // se qtdPartes == qtdMaquinas, então mover uma maquina também é impossível
                    // escolha entre swapParte, swapMaquina, splitCcluster(caso qtdClusters = 1) ou unionClusters(caso qtdCluster = max)
                    
                    int opInterna = intervalRand(1, 60);
                    if (s.qtdMaquinas != s.qtdPartes) opInterna = intervalRand(1, 80); // caso em que se pode mover uma maquina

                    if (sLinha.qtdClusters == 1) {
                        sLinha.splitPiorCluster();
                    } else if (opInterna <= 20) {
                        sLinha.unionPiorCluster();
                    } else if (opInterna <= 40) {
                        sLinha.swapPioresPartes2();
                    } else if (opInterna <= 60) {
                        sLinha.swapPioresMaquinas2();
                    } else {
                        sLinha.moverPiorMaquina2();
                    }
                }

            } else if (op <= 80) {
                // swapPioresMaquinas2

                if (sLinha.swapPioresMaquinas2() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitPiorCluster();
                }

            } else if (op <= 90) {
                // swapPioresPartes2

                if (sLinha.swapPioresPartes2() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitPiorCluster();
                }

            } else if (op <= 95) {
                // swapMaquinas
                
                if (sLinha.swapMaquinas() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitPiorCluster();
                }

            } else {
                // swapParte
                
                if (sLinha.swapPartes() == 0) {
                    // caso em que só há um cluster
                    // ou seja, é preciso usar split para alterar a eficacia
                    sLinha.splitPiorCluster();
                }

            }

            sLinha.getFObj();
            float delta = sLinha.eficacia - s.eficacia;
            

            if (contador > PAmax) {
                PAmax *= 2;
                contador = 0;
                T = T * 1.0005;

                // perturbação
                s = sMelhor;
                for (int i = 0; i < 4; i++) {
                    s.unionClusterPiorSomaOut();

                    if (s.eficacia > sMelhor.eficacia) {
                        sMelhor = s;
                    }
                }

                if (s.qtdClusters < min(s.qtdMaquinas, s.qtdPartes)) {
                    s.splitPiorCluster();
                    s.unionCluster();
                } else {
                    s.unionCluster();
                    s.splitPiorCluster();
                }
                
                for (int i = 0; i < 4; i++) {
                    int aux = intervalRand(0, 1);

                    if (aux == 0) s.perturbaMaquinas();
                    else s.perturbaPartes();
                }
            } else if (delta > 0.0) {
                
                s = sLinha;
                if (s.eficacia > sMelhor.eficacia) {
                    contador = 0;
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