#include "snippets.h"
#include <stdlib.h>

int intervalRand(int l, int r) {
    return l + (rand() % (r - l + 1));
}
