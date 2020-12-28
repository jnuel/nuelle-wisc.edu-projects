#include <stdio.h>/* many odd sized allocations */
#include <assert.h>
#include <stdlib.h>
#include "mem.h"

int main() {
   assert(Init_Mem(8) == 0);
   assert(Alloc_Mem(1) != NULL);
   printf("%s", "passed");
   exit(0);
}