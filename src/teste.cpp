#include <bits/stdc++.h>
#include "solucao.h"

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    solucao s1(n, m);

    s1.atualizaMatriz();
    s1.exibeMatriz();

    int trocas;
    cin >> trocas;
    while(trocas--) {
        int i, j;
        cin >> i >> j;
        s1.trocarLinhas(i, j);
        s1.exibeMatriz();
    }

    cout << s1.num_1 << endl;

    return 0;
}