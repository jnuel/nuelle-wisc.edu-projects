#include <stdio.h>/* allocation is too big to fit in available space */
#include <assert.h>
#include <stdlib.h>
#include "mem.h"

int main() {
    assert(Init_Mem(4096) == 0);
    assert(Alloc_Mem(4095) == NULL);
    printf("%s", "passed");
    exit(0);
}