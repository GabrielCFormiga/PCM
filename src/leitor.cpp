#include "leitor.h"
#include <iostream>
#include <sstream>

using namespace std;

leitor::leitor() { 
    string linha;

    getline(cin, identificador);

    // ignora comentários e a linha vazia
    while (getline(cin, linha)) {
        if (linha.empty()) break;
    }

    int qtdM, qtdP;
    cin >> qtdM >> qtdP;
    cin.ignore();

    // define o tamanho da matriz de acordo com qtdM e qtdP
    matriz.resize(qtdM);
    for (int i = 0; i < qtdM; i++) {
        matriz[i].resize(qtdP);
    }

    // faz a leitura da linha de cada máquina i
    for (int i = 1; i <= qtdM; i++) {
        getline(cin, linha);

        stringstream ss(linha);
        int parte;
        vector<int> partes;

        ss >> parte;
        while (ss >> parte) {
            partes.push_back(parte);
        }

        // conecta as partes lidas à máquina i
        for (int j = 0; j < partes.size(); j++) {
            matriz[i - 1][partes[j] - 1] = 1;
        }
    }

}
