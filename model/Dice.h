#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace Dice {
static void Seed(int s = time(NULL)) {
    srand(s);
}

static int Roll() {
    return rand() % 6 + 1;
}
}
