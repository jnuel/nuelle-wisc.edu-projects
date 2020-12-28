#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <dirent.h>
#include <stdbool.h>

#define stat xv6_stat  // avoid clash with host struct stat
#define dirent xv6_dirent  // avoid clash with host struct stat
#include "types.h"
#include "fs.h"
#include "stat.h"
#undef stat
#undef dirent
int main(int argc, char *argv[])
{
    uchar buf[8];
    bzero(buf, 8);
    for (int i = 0; i < 7; i++)
    {
        buf[i / 8] = buf[i / 8] | (0x1 << (i % 8));
    }
    for (int a = 0; a < 10; a++) {
        printf("%d\n", buf[a/8] >> (a % 8) & 1);
    }
}