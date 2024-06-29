#include "snippets.h"
#include <stdlib.h>

// gera números aleatórios inteiros em [l, r]
int intervalRand(int l, int r) {
    return l + (rand() % (r - l + 1));
}

// gera números aleatórios de ponto flutuante em [0, 1)
float randomFloat() {
    return (float)(rand()) / (float)(RAND_MAX);
}