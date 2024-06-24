#pragma once

class cluster {
    public:
        // atributos
        int itl, jtl, ibr, jbr; // tl = top left, br = bottom right
        int num_buracos;
        int pior_linha, num_zeros_pior_linha;
        int pior_coluna, num_zeros_pior_coluna; // pior = maior numero de buracos
        float percentual_de_buracos;

        // construtores
        cluster();
        cluster(int itl, int jtl, int ibr, int jbr);
        ~cluster();

        // metodos
        void atualizaTL(int i, int j);
        void atualizaBR(int i, int j);

};
