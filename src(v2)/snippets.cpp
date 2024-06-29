#include "snippets.h"
#include <stdlib.h>

// gera numeros inteiros em [l, r]
int intervalRand(int l, int r) {
    return l + (rand() % (r - l + 1));
}

float randomFloat() {
    return (float)(rand()) / (float)(RAND_MAX);
}

// gera numeros reais em [a, b + 1)
float intervalFloatRand(int a, int b) {
    return (float) intervalRand(a, b) + randomFloat();
}