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

void check1(struct superblock *sb)
{
  //filesystem_size_in_superblock >= num_data_blocks + num_blocks_for_inode + num_blocks_for_bitmap + 1 (for superblock)
  int min = INT_MAX;
  int max = INT_MIN;
  //printf("min is %d max is %d", min, max);
  int curblock = 0;
  for (int numbloc = 0; numbloc < sb->ninodes; numbloc++)
  {
    curblock = BBLOCK(numbloc, sb->ninodes);
    if (curblock < min)
    {
      min = curblock;
    }
    else if (curblock > max)
    {
      max = curblock;
    }
  }
  if (max - min == 0)
  {
    if (sb->size < (sb->nblocks + (sb->ninodes / IPB) + 4))
    {
      fprintf(stderr, "%s", "ERROR: superblock is corrupted.\n");
      exit(1);
    }
    else
    {
      if (sb->size < (sb->nblocks + (sb->ninodes / IPB) + (max - min) + 3))
      {
        fprintf(stderr, "%s", "ERROR: superblock is corrupted.\n");
        exit(1);
      }
    }
  }
}

void check2(struct superblock *sb, struct dinode *dip)
{
  for (int i = 1; i < sb->ninodes; i++)
  {
    // printf("type %d\n", dip[i].type);
    if (dip[i].type != 0)
      if (dip[i].type != 1)
        if (dip[i].type != 2)
          if (dip[i].type != 3)
          {
            fprintf(stderr, "%s", "ERROR: bad inode.\n");
            exit(1);
          }
  }
}

void check3(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  for (int i = 1; i < sb->ninodes; i++)
  {
    short addr = 0;
    if (dip[i].size > 0)
    {
      for (int a = 0; a < NDIRECT; a++)
      {
        addr = dip[i].addrs[a];
        //printf("%d < (sb->ninodes/IPB+4) = %ld, > %d\n",  addr, sb->ninodes/IPB + 4, sb->size);
        if (addr != 0)
        {
          if ((addr < (sb->ninodes / IPB) + 4) || (addr > sb->size))
          {
            //   printf("sb->size %d, data is %d inodes is %ld and addr is %d, first is %ld, second is %d\n", sb->size, sb->nblocks, sb->ninodes/IPB, addr, sb->ninodes/IPB + 4, (sb->size - sb->nblocks));
            fprintf(stderr, "%s", "ERROR: bad direct address in inode.\n");
            exit(1);
          }
        }
      }

      //  // printf("IN INDIRECT                                            \n");
      uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[i].addrs[NDIRECT]);
      //  printf("we are at inode %d, %d\n", i, dip[i].addrs[NDIRECT]);
      if (dip[i].addrs[NDIRECT] != 0)
      {
        if ((dip[i].addrs[NDIRECT] < (sb->ninodes / IPB) + 4) || (dip[i].addrs[NDIRECT] > sb->size))
        {
          fprintf(stderr, "%s", "ERROR: bad indirect address in inode.\n");
          exit(1);
        }

        for (int i = 1; i < BSIZE / sizeof(uint); ++i)
        {
          if (cat_indirect[i] != 0)
          {
            if ((cat_indirect[i] < (sb->ninodes / IPB) + 4) || (cat_indirect[i] > sb->size))
            {
              fprintf(stderr, "%s", "ERROR: bad indirect address in inode.\n");
              exit(1);
            }
          }
        }
      }
    }
  }
}

void check4(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  for (int i = 1; i < sb->ninodes; i++)
  {
    if (dip[i].type == 1)
    {
      uint data_block_addr = dip[i].addrs[0]; // 29
      //printf("data_block_addr for / is %d\n", data_block_addr);
      struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + data_block_addr * BSIZE);
      // if (i == 1) {
      //printf("%s %s\n", (entry[0].name), entry[1].name);
      if ((strcmp(entry[0].name, ".") != 0) || (strcmp(entry[1].name, "..") != 0))
      {
        fprintf(stderr, "%s", "ERROR: directory not properly formatted.\n");
        exit(1);
      }
      if (entry[0].inum != i)
      {
        // printf("here");
        fprintf(stderr, "%s", "ERROR: directory not properly formatted.\n");
        exit(1);
      }
      if (i == 1 && (entry[0].inum != i) && (entry[1].inum != i))
      {
        // printf("here");
        fprintf(stderr, "%s", "ERROR: directory not properly formatted.\n");
        exit(1);
      }
    }
  }
}

void check5(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  for (int i = 1; i < sb->ninodes; i++)
  {
    short addr = 0;
    if (dip[i].size > 0)
    {
      for (int a = 0; a < NDIRECT; a++)
      {
        addr = dip[i].addrs[a];
        //printf("%d < (sb->ninodes/IPB+4) = %ld, > %d\n",  addr, sb->ninodes/IPB + 4, sb->size);
        if (addr != 0)
        {
          char *start = (char *)(img_ptr + BSIZE * BBLOCK(addr, sb->ninodes));
          // printf("%ld\n", BBLOCK(addr,sb->ninodes));
          // printf("i:%d, addr:%d, %d\n", i, addr, start[addr / 8] >> (addr % 8) & 1);
          if ((start[addr / 8] >> (addr % 8) & 1) == 0)
          {
            fprintf(stderr, "%s", "ERROR: address used by inode but marked free in bitmap.\n");
            exit(1);
          }
          // check the bitmap
        }
      }

      uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[i].addrs[NDIRECT]);
      //  printf("we are at inode %d, %d\n", i, dip[i].addrs[NDIRECT]);
      if (dip[i].addrs[NDIRECT] != 0)
      {
        if ((dip[i].addrs[NDIRECT] < (sb->ninodes / IPB) + 4) || (dip[i].addrs[NDIRECT] > sb->size))
        {
          fprintf(stderr, "%s", "ERROR: bad indirect address in inode.\n");
          exit(1);
        }

        for (int i = 1; i < BSIZE / sizeof(uint); ++i)
        {
          if (cat_indirect[i] != 0)
          {
            char *start = (char *)(img_ptr + BSIZE * BBLOCK(cat_indirect[i], sb->ninodes));
            if ((start[cat_indirect[i] / 8] >> (cat_indirect[i] % 8) & 1) == 0)
            {
              fprintf(stderr, "%s", "ERROR: address used by inode but marked free in bitmap.\n");
              exit(1);
            }
          }
        }
      }
    }
  }
}

void check6(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  int min = INT_MAX;
  int max = INT_MIN;
  //printf("min is %d max is %d", min, max);
  int curblock = 0;
  for (int numbloc = 0; numbloc < sb->ninodes; numbloc++)
  {
    curblock = BBLOCK(numbloc, sb->ninodes);
    if (curblock < min)
    {
      min = curblock;
    }
    else if (curblock > max)
    {
      max = curblock;
    }
  }

  if (min == max)
  {
    char *start = (char *)(img_ptr + BSIZE * BBLOCK(0, sb->ninodes));
    int found = 0;
    // printf("%ld\n", (sb->ninodes/IPB));
    for (int bit = 4 + ((sb->ninodes) / IPB); bit < BPB; bit++) // start at 29 because 28 before are all set to 1.
    {
      if ((start[bit / 8] >> (bit % 8) & 1) != 0)
      {
        found = 0;
        // printf("bit: %d\n", bit);
        for (int curinode = 1; curinode < sb->ninodes; curinode++)
        {
          short addr = 0;
          if (dip[curinode].size > 0)
          {
            for (int curdir = 0; curdir < NDIRECT; curdir++)
            {
              // printf("x:%d a:%d dip[i].addrs[a]:%d\n", x, a, dip[x].addrs[a]);
              if (dip[curinode].addrs[curdir] == bit)
              {
                found = 1;
                break; // break out of searching through directs
              }
            }
            if (found == 1)
            {
              break;
            } // don't check the indirect blocks
            // check the indirect blocks
            uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[curinode].addrs[NDIRECT]);
            //  printf("we are at inode %d, %d\n", i, dip[i].addrs[NDIRECT]);
            if (dip[curinode].addrs[NDIRECT] != 0 && dip[curinode].addrs[NDIRECT] == bit)
            {
              found = 1;
              break;
            }

            if (dip[curinode].addrs[NDIRECT] != 0)
            {
              for (int curindir = 0; curindir < NINDIRECT; ++curindir)
              {
                if (cat_indirect[curindir] != 0 && cat_indirect[curindir] == bit)
                {
                  found = 1;
                  break; // break out of searching thru indirects
                }
              }
            }
          }
          if (found == 1)
          {
            break;
          } // break out of searching thru each inode
        }
        if (found == 0)
        {
          // printf("bit we found :%d\n", bit);
          fprintf(stderr, "%s", "ERROR: bitmap marks block in use but it is not in use.\n");
          exit(1);
        }
      }
    }
  }
  else
  {
    while (min != max)
    {
      char *start = (char *)(img_ptr + BSIZE * min);
      int found = 0;
      // printf("%ld\n", (sb->ninodes/IPB));
      for (int bit = 4 + ((sb->ninodes) / IPB); bit < BPB; bit++) // start at 29 because 28 before are all set to 1.
      {
        if ((start[bit / 8] >> (bit % 8) & 1) != 0)
        {
          found = 0;
          // printf("bit: %d\n", bit);
          for (int curinode = 1; curinode < sb->ninodes; curinode++)
          {
            short addr = 0;
            if (dip[curinode].size > 0)
            {
              for (int curdir = 0; curdir < NDIRECT; curdir++)
              {
                // printf("x:%d a:%d dip[i].addrs[a]:%d\n", x, a, dip[x].addrs[a]);
                if (dip[curinode].addrs[curdir] == bit)
                {
                  found = 1;
                  break; // break out of searching through directs
                }
              }
              if (found == 1)
              {
                break;
              } // don't check the indirect blocks
              // check the indirect blocks
              uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[curinode].addrs[NDIRECT]);
              //  printf("we are at inode %d, %d\n", i, dip[i].addrs[NDIRECT]);
              if (dip[curinode].addrs[NDIRECT] != 0 && dip[curinode].addrs[NDIRECT] == bit)
              {
                found = 1;
                break;
              }

              if (dip[curinode].addrs[NDIRECT] != 0)
              {
                for (int curindir = 0; curindir < NINDIRECT; ++curindir)
                {
                  if (cat_indirect[curindir] != 0 && cat_indirect[curindir] == bit)
                  {
                    found = 1;
                    break; // break out of searching thru indirects
                  }
                }
              }
            }
            if (found == 1)
            {
              break;
            } // break out of searching thru each inode
          }
          if (found == 0)
          {
            // printf("bit we found :%d\n", bit);
            fprintf(stderr, "%s", "ERROR: bitmap marks block in use but it is not in use.\n");
            exit(1);
          }
        }
      }
      min++;
    }
  }
}

void check7(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  int data[sb->ninodes * 12];
  int f = 0;
  for (int i = 1; i < sb->ninodes; i++)
  {
    short addr = 0;
    if (dip[i].size > 0)
    {
      for (int a = 0; a < NDIRECT; a++)
      {
        addr = dip[i].addrs[a];
        //printf("%d < (sb->ninodes/IPB+4) = %ld, > %d\n",  addr, sb->ninodes/IPB + 4, sb->size);
        if (addr != 0)
        {
          for (int b = 0; b < f; b++)
          {
            if (data[b] == addr)
            {
              fprintf(stderr, "%s", "ERROR: direct address used more than once.\n");
              exit(1);
            }
          }
          data[f] = addr;
          f++;
        }
      }
    }
  }
}

void check8(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  // printf("%ld\n", sb->ninodes/IPB);
  for (int i = 0; i < sb->ninodes; i++)
  {
    short b = 0;
    if (dip[i].size > 0)
    {
      for (int a = 0; a < NDIRECT; a++)
      {
        if (dip[i].addrs[a] != 0)
        {
          b++;
        }
      }
      if (dip[i].addrs[NDIRECT] != 0)
      {
        // printf("%d \n", dip[i].addrs[NDIRECT-1]);
        //  printf("b is %d and we are at inode %d, %d %d\n", b, i, dip[i].addrs[NDIRECT], NDIRECT);
        uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[i].addrs[NDIRECT]);
        // printf("%ld %d \n", (sb->ninodes / IPB) + 4, sb->size);

        if ((dip[i].addrs[NDIRECT] < (sb->ninodes / IPB) + 4) || (dip[i].addrs[NDIRECT] > sb->size))
        {
          //   printf("here");
          fprintf(stderr, "%s", "ERROR: bad indirect address in inode.\n");
          exit(1);
        }

        for (int i = 0; i < NINDIRECT; ++i)
        {
          // printf("%d\n", cat_indirect[i]);
          if (cat_indirect[i] != 0)
          {
            b++;
            //     printf("%d and b is %d\n", cat_indirect[i], b);
          }
        }
      }

      if ((dip[i].size <= ((b - 1) * BSIZE) || (dip[i].size > b * BSIZE)))
      {
        //  printf("dip[i].size %d <= %d || %d > %d\n", dip[i].size, ((b - 1) * BSIZE), dip[i].size, b * BSIZE);
        fprintf(stderr, "%s", "ERROR: incorrect file size in inode.\n");
        exit(1);
      }
    }
  }
}

void check9(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  int data[sb->ninodes];
  for (int place = 0; place < sb->ninodes; place++)
  {
    data[place] = 0;
  }

  int found = 0;
  for (int i = 1; i < sb->ninodes; i++)
  {
    if (dip[i].type == 1)
    {
      for (int curdir = 0; curdir < NDIRECT; curdir++)
      {
        if (dip[i].addrs[curdir] != 0)
        {
          struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + dip[i].addrs[curdir] * BSIZE);
          for (int direntry = 0; direntry < BSIZE / (2 + DIRSIZ); direntry++)
          {
            if (entry[direntry].inum != 0)
            {
              data[entry[direntry].inum] = 1;
            }
          }
        }
      }
      if (dip[i].addrs[NDIRECT] != 0)
      {
        uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[i].addrs[NDIRECT]);
        for (int numindir = 0; numindir < NINDIRECT; numindir++)
        {
          if (cat_indirect[numindir] != 0)
          {
            struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + cat_indirect[numindir] * BSIZE);
            for (int direntry = 0; direntry < BSIZE / (2 + DIRSIZ); direntry++)
            {
              if (entry[direntry].inum != 0)
              {
                data[entry[direntry].inum] = 1;
              }
            }
          }
        }
      }
    }
  }

  for (int curinode = 1; curinode < sb->ninodes; curinode++)
  {
    if (dip[curinode].type != 0)
    {
      // printf("at %d, ", curinode);
      // printf("%d\n", data[curinode]);
      if (data[curinode] == 0)
      { // then there is an error
        //   printf("error here %d\n", curinode);
        fprintf(stderr, "%s", "ERROR: inode marked used but not found in a directory.\n");
        exit(1);
      }
    }
  }
}

void check10(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  int found = 0;
  for (int i = 1; i < sb->ninodes; i++)
  {
    if (dip[i].type == 1)
    {
      struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + dip[i].addrs[0] * BSIZE);
      //inode_num=entry[I].inum; dip[inode_num] should not have type==0.
      while (entry->inum != 0)
      {
        if (dip[entry->inum].type == 0)
        {
          fprintf(stderr, "%s", "ERROR: inode referred to in directory but marked free.\n");
          exit(1);
        }
        entry++;
      }
    }
  }
}

void check11(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  int data[sb->ninodes];
  for (int place = 0; place < sb->ninodes; place++)
  {
    data[place] = 0;
  }

  int found = 0;
  for (int i = 1; i < sb->ninodes; i++)
  {
    if (dip[i].type == 1)
    {
      for (int curdir = 0; curdir < NDIRECT; curdir++)
      {
        if (dip[i].addrs[curdir] != 0)
        {
          struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + dip[i].addrs[curdir] * BSIZE);
          for (int direntry = 0; direntry < BSIZE / (2 + DIRSIZ); direntry++)
          {
            if (entry[direntry].inum != 0)
            {
              data[entry[direntry].inum]++;
            }
          }
        }
      }
      if (dip[i].addrs[NDIRECT] != 0)
      {
        uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[i].addrs[NDIRECT]);
        for (int numindir = 0; numindir < NINDIRECT; numindir++)
        {
          if (cat_indirect[numindir] != 0)
          {
            struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + cat_indirect[numindir] * BSIZE);
            for (int direntry = 0; direntry < BSIZE / (2 + DIRSIZ); direntry++)
            {
              if (entry[direntry].inum != 0)
              {
                data[entry[direntry].inum]++;
              }
            }
          }
        }
      }
    }
  }
  for (int curinode = 1; curinode < sb->ninodes; curinode++)
  {
    if (dip[curinode].type == 2)
    {
      if (dip[curinode].nlink != data[curinode])
      {
        // printf("we have an error at curinode:%d\n", curinode);
        fprintf(stderr, "%s", "ERROR: bad reference count for file.\n");
        exit(1);
      }
    }
  }
}

void check12(struct superblock *sb, struct dinode *dip, void *img_ptr)
{
  int data[sb->ninodes];
  for (int place = 0; place < sb->ninodes; place++)
  {
    data[place] = 0;
  }

  int found = 0;
  for (int i = 1; i < sb->ninodes; i++)
  {
    if (dip[i].type == 1)
    {
      for (int curdir = 0; curdir < NDIRECT; curdir++)
      {
        if (dip[i].addrs[curdir] != 0)
        {
          struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + dip[i].addrs[curdir] * BSIZE);
          for (int direntry = 0; direntry < BSIZE / (2 + DIRSIZ); direntry++)
          {
            if (entry[direntry].inum != 0 && !(strcmp(entry[direntry].name, ".") == 0 || strcmp(entry[direntry].name, "..") == 0))
            {
             // printf("at inum:%d name:%s at i:%d curdir:%d direntry:%d \n", entry[direntry].inum, entry[direntry].name, i, curdir, direntry);
              data[entry[direntry].inum]++;
            }
          }
        }
      }
      if (dip[i].addrs[NDIRECT] != 0)
      {
        uint *cat_indirect = (uint *)(img_ptr + BSIZE * dip[i].addrs[NDIRECT]);
        for (int numindir = 0; numindir < NINDIRECT; numindir++)
        {
          if (cat_indirect[numindir] != 0)
          {
            struct xv6_dirent *entry = (struct xv6_dirent *)(img_ptr + cat_indirect[numindir] * BSIZE);
            for (int direntry = 0; direntry < BSIZE / (2 + DIRSIZ); direntry++)
            {
              if (entry[direntry].inum != 0 && !(strcmp(entry[direntry].name, ".") == 0 || strcmp(entry[direntry].name, "..") == 0))
              {
                // printf("found this INDIRECT inum:%d name:%s at i:%d numindir:%d direntry:%d \n", entry[direntry].inum, entry[direntry].name, i, numindir, direntry);
                data[entry[direntry].inum]++;
              }
            }
          }
        }
      }
    }
  }
  for (int hi = 2; hi < sb->ninodes; hi++)
  {
    int tofind = data[hi];
    // while (tofind > 1) {
    //   for (int curinode = 1; curinode < sb->ninodes; curinode++) {

    //   }
    // }
    if (data[hi] > 1 && data[hi] != dip[hi].nlink)
    {
      fprintf(stderr, "%s", "ERROR: directory appears more than once in file system.\n");
      exit(1);
    }
    //printf("found this inum:%d this many times %d, nlink:%d\n", hi, data[hi],dip[hi].nlink );
  }

  // for (int curinode = 1; curinode < sb->ninodes; curinode++)
  // {
  //   if (dip[curinode].type == 2)
  //   {
  //     if (dip[curinode].nlink != data[curinode])
  //     {
  //       // printf("we have an error at curinode:%d\n", curinode);
  //       fprintf(stderr, "%s", "ERROR: bad reference count for file.\n");
  //       exit(1);
  //     }
  //   }
  // }
}

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
    fprintf(stderr, "%s", "Usage: xfsck <file_system_image>\n");
    exit(1);
  }

  if (fd < 0)
  {
    fprintf(stderr, "%s", "image not found.\n"); // Error message on stderr (using fprintf)
    exit(1);
  }

  struct stat sbuf;
  fstat(fd, &sbuf);
  // printf("Image that i read is %ld in size\n", sbuf.st_size);

  // mmap
  void *img_ptr = mmap(NULL, sbuf.st_size,
                       PROT_READ, MAP_PRIVATE, fd, 0);

  // img_ptr is pointing to byte 0 inside the file
  // FS layout:
  // unused | superblock | inodes
  // first 512 | next 512 is super block
  struct superblock *sb = (struct superblock *)(img_ptr + BSIZE);
  // printf("size %d nblocks %d ninodes %d\n", sb->size, sb->nblocks, sb->ninodes);

  struct dinode *dip = (struct dinode *)(img_ptr + 2 * BSIZE);
  check1(sb);
  check2(sb, dip);
  check3(sb, dip, img_ptr);
  check4(sb, dip, img_ptr);
  check5(sb, dip, img_ptr);
  check6(sb, dip, img_ptr);
  check7(sb, dip, img_ptr);
  check8(sb, dip, img_ptr);
  check9(sb, dip, img_ptr);
  check10(sb, dip, img_ptr);
  check11(sb, dip, img_ptr);
  check12(sb, dip, img_ptr);
}