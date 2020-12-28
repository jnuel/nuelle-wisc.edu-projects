#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>
#include <string.h>

#define stat xv6_stat     // avoid clash with host struct stat
#define dirent xv6_dirent // avoid clash with host struct stat
#include "types.h"
#include "fs.h"
#include "stat.h"
#undef stat
#undef dirent

// xv6 fs img similar to vsfs
// unused | superblock | inode table | unused | bitmap (data) | data blocks

void print_inode(struct dinode dip);

int main(int argc, char *argv[])
{
  int fd;
  // Usage is something like <my prog> <fs.img>
  if (argc == 2)
  {
    fd = open(argv[1], O_RDONLY);
  }
  else
  {
    printf("Usage: program fs.img\n");
    exit(1);
  }

  if (fd < 0)
  {
    printf("Usage: %s file not found\n", argv[1]);
    exit(1);
  }

  struct stat sbuf;
  fstat(fd, &sbuf);
  printf("Image that i read is %ld in size\n", sbuf.st_size);

  // mmap
  void *img_ptr = mmap(NULL, sbuf.st_size,
                       PROT_READ, MAP_PRIVATE, fd, 0);

  // check if img_ptr is not -1
  //
  // img_ptr is pointing to byte 0 inside the file
  // FS layout:
  // unused | superblock | inodes
  // first 512 | next 512 is super block
  struct superblock *sb = (struct superblock *)(img_ptr + BSIZE);
  printf("size %d nblocks %d ninodes %d\n", sb->size, sb->nblocks, sb->ninodes);
  //
  // inode table, bitmaps?
  //    inode number for / is 1. 0 is unused ?!
  //    inode table starts from block 2 (block 0 is unused, block 1 is SB
  //
  struct dinode *dip = (struct dinode *)(img_ptr + 2 * BSIZE);
  print_inode(dip[0]);
  print_inode(dip[1]);

  for (int inodenum = 0; inodenum < sb->ninodes; inodenum++)
  {
    if (dip[inodenum].type == 1)
    {
      for (int hi = 0; hi < NDIRECT; hi++)
      {
        uint data_block_addr = dip[inodenum].addrs[hi]; // 29
                                                        // printf("data_block_addr for / is %d\n", data_block_addr);
        struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + data_block_addr * BSIZE);
        //printf("%d\n", strcmp(entry[0].name, "."));
        for (int i = 0; i < BSIZE / (2 + DIRSIZ); ++i)
        {
          if (entry[i].inum != 0)
            printf("name is %s inum is %d, %d\n", entry[i].name, entry[i].inum, inodenum);
        }
      }
    }

    if (dip[inodenum].addrs[NDIRECT] != 0)
    {
      uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[inodenum].addrs[NDIRECT]);
      for (int numindir = 0; numindir < NINDIRECT; numindir++)
      {
        if (cat_indirect[numindir] != 0)
        {
          struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + cat_indirect[numindir] * BSIZE);
          for (int direntry = 0; direntry < BSIZE / (2 + DIRSIZ); direntry++)
          {
            if (entry[direntry].inum != 0)
            {
              printf("name is %s inum is %d, %d\n", entry[direntry].name, entry[direntry].inum, inodenum);
            }
          }
        }
      }
    }
  }
}
// for (int hi = 0; hi < NDIRECT; hi++) {
//   uint data_block_addr = dip[1].addrs[hi]; // 29
//   printf("data_block_addr for / is %d\n", data_block_addr);
// 	struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + data_block_addr * BSIZE);
//   //printf("%d\n", strcmp(entry[0].name, "."));
//   for (int i = 0; i < DIRSIZ; ++i) {
//     printf("name is %s inum is %d\n", entry[i].name, entry[i].inum);
//   }
// }

// print addrs for inode number 2
// print_inode(dip[2]);
// printf("the ndirect addr for /README are\n");
// for (int i = 0; i < NDIRECT; ++i) {
//   printf("%d ", dip[2].addrs[i]);
// }
// printf("\n");

// char* readme_ptr = (char*) (img_ptr + BSIZE * dip[2].addrs[0]);
// printf("first few characters of /README are\n");
// for(int i = 0; i < 8; ++i) {
//   printf("%c", readme_ptr[i]);
// }
// readme_ptr = (img_ptr + BSIZE * dip[2].addrs[1]);
// printf("\n");

// indirect blocks
// for (int i = 0; i < sb->ninodes; i++) {
//   printf("%d", i);
//   print_inode(dip[i]);
// }

// printf("the ndirect addr for /cat are\n");
// for (int i = 0; i < NDIRECT+1; ++i) {
//   printf("%d ", dip[3].addrs[i]);
// }
// printf("\n");

// // indirect pointer contains a list of addrs
// // uints in our case
// uint* cat_indirect = (uint*) (img_ptr + BSIZE * dip[4].addrs[NDIRECT]);
// printf("the contents of cat_indirect are \n");
//BSIZE = 512
//  <addr> = 4 bytes
//  <addr>
//  <addr>
//....
// for (int i = 0; i < BSIZE/sizeof(uint); ++i) {
//   printf("%d ", cat_indirect[i]);
// }
// printf("\n");

// Bitmaps??
//printf("Number of inodes %d size of IPB %ld\n", sb->ninodes, IPB);

// FS layout:
// unused | superblock | inodes - 25 blocks | unused | bitmap | data
// 01234....2627<28>
// 1 + 1 + 25 + 1 + 1 + 995 = 1024 blocks


void print_inode(struct dinode dip)
{
  printf("file type:%d,", dip.type);
  printf("nlink:%d,", dip.nlink);
  printf("size:%d,", dip.size);
  printf("first_addr:%d\n", dip.addrs[0]);
}
