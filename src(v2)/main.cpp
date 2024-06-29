#include <iostream>
#include <math.h>
#include "solucao.h"
#include "snippets.h"

using namespace std;

const float e = 2.7182818284590;

int main() {
    // define a matriz do problema
    int n, m;
    cin >> n >> m;

    solucao s(n, m);
    s.setMatriz();
    
    // começo do SA
    solucao sMelhor = s;
    float alpha = 0.98; // coeficiente de resfriamento
    int iterT = 0, T = 900; // número de iterações na temperatura T 
    int SAmax = 50;

    while (T > 0) {
        while (iterT < SAmax) {
            iterT++;

            int op = intervalRand(0, 100);
            solucao sLinha = s;

            if (op <= 10) {
                // union
                if (sLinha.unionCluster() == 0) {
                    sLinha.swapMaquinas();
                }
            } else if (op <= 20) {
                // split
                if (sLinha.splitCluster() == 0) {
                    sLinha.swapPartes();
                }
            } else if (op <= 40) {
                // moverMaquina
                if (sLinha.moverMaquina() == 0) {
                    sLinha.swapMaquinas();
                }
            } else if (op <= 60) {
                // moverParte
                if (sLinha.moverParte() == 0) {
                    s.swapPartes();
                }
            } else if (op <= 80) {
                // swapMaquinas
                sLinha.swapMaquinas();
            } else {
                // swapParte
                sLinha.swapPartes();
            }

            float delta = sLinha.getFObj() - s.eficacia;
            
            if (delta > 0) {
                s = sLinha;
                if (s.eficacia > sMelhor.eficacia) {
                    sMelhor = s;
                }
            } else {
                float x = intervalFloatRand(0, 0);
                float aux = pow(e, -delta / T);
                if (x < aux) s = sLinha;
            }
        }
        
        T = alpha * T;
        iterT = 0;
    }

    cout << sMelhor.eficacia << endl;
}
