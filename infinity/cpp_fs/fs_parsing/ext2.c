
#include <unistd.h>
#include <stdlib.h>
#include <ext2fs/ext2_fs.h>
#include <ext2fs/ext2fs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* #include <sys/mount.h> */
#include <assert.h>
#include <dirent.h>


#include "ext2.h"

#define BASE_OFFSET (1024) /* location of the super-block in the first group */
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block - 1) * 4096)

/*------------------------------------------------------------------------------*/

static void ReadSuperblock(int fd, struct ext2_super_block *sb);
static void PrintSuperBlock(struct ext2_super_block *sb);
static void ReadGroupDescriptor(int fd, struct ext2_group_desc *gd,struct ext2_super_block *sb);
static size_t ConvertLogToNum(size_t log);
static void PrintGroupDescriptor(struct ext2_group_desc *gd);
static void ReadINode(int fd, int inode_num, struct ext2_inode *in, struct ext2_group_desc *gd);
static void ReadDirectory(int fd, struct ext2_dir_entry_2 *de, struct ext2_inode *in);
static void PrintRootDirectory(int fd, struct ext2_inode *in, struct ext2_group_desc *gd);
static void PrintInode(int fd, struct ext2_inode *in);
static size_t GetGroupNum(struct ext2_super_block *sb);
static void  PrintFileList(const char *path);
static void PrintFileContent(const char *filepath);

FILE *Find(const char *dev, const char *file_path)
{
  /*check if the path is availbale*/
  size_t block_size;
  int fd = 0;
  size_t num_of_groups;
  struct ext2_super_block sb;
  struct ext2_group_desc gd;
  struct ext2_inode root_in;
  unsigned char *bitmap;


  fd = open(dev,O_RDONLY);
  if(0 > fd)
  {
    printf("unable to open dev");
  }
  ReadSuperblock(fd,&sb);
  block_size = ConvertLogToNum(sb.s_log_block_size);

  PrintSuperBlock(&sb);
  ReadGroupDescriptor(fd,&gd,&sb);
  PrintGroupDescriptor(&gd);

  ReadINode(fd, EXT2_ROOT_INO, &root_in, &gd);

  PrintRootDirectory(fd, &root_in, &gd);
  PrintFileList(sb.s_last_mounted);
  PrintFileContent(file_path);
  return 0;
}

static void ReadSuperblock(int fd, struct ext2_super_block *sb)
{

   /*  struct superblock *sb = malloc(sizeof(struct superblock)); */
    assert(sb != NULL);

  lseek(fd, BASE_OFFSET, SEEK_SET);
  read(fd, (void *)sb, sizeof(struct ext2_super_block));

}
static size_t ConvertLogToNum(size_t log)
{
  return 1024 << log;
}
static void PrintSuperBlock(struct ext2_super_block *sb)
{
  printf("\nSuperblock:\n");
  printf(" directory where last mounted: %s\n",sb->s_last_mounted);
  printf(" s_inodes_count: %d\n", sb->s_inodes_count);
  printf(" s_blocks_count: %d\n", sb->s_blocks_count);
  printf(" s_free_blocks_count: %d\n", sb->s_free_blocks_count);
  printf(" s_free_inodes_count: %d\n", sb->s_free_inodes_count);
  printf(" s_first_data_block: %d\n", sb->s_first_data_block);
  printf(" s_inodes_per_group: %d\n", sb->s_inodes_per_group);

  printf(" s_block_size: %ld\n", ConvertLogToNum(sb->s_log_block_size));
  printf("---------------------------\n");
}
static size_t GetGroupNum(struct ext2_super_block *sb)
{
  size_t num_of_groups;
  num_of_groups = 1 + (sb->s_blocks_count - 1) / sb->s_blocks_per_group;
  return num_of_groups;
}

static void ReadGroupDescriptor(int fd, struct ext2_group_desc *gd,struct ext2_super_block *sb)
{
  size_t offset = (1 + sb->s_first_data_block) * ConvertLogToNum(sb->s_log_block_size);
  printf("block size %ld\n",ConvertLogToNum(sb->s_log_block_size));

  lseek(fd, offset, SEEK_SET); /* position head above the group descriptor block */
  read(fd, (void *)gd, sizeof(struct ext2_group_desc));
  
}
static void PrintGroupDescriptor(struct ext2_group_desc *gd)
{
  printf("\nGroupDescriptor:\n");
  printf(" bg_block_bitmap: %d\n", gd->bg_block_bitmap);
  printf(" bg_inode_bitmap: %d\n", gd->bg_inode_bitmap);
  printf(" bg_inode_table: %d\n", gd->bg_inode_table);
  printf(" bg_free_blocks_count: %d\n", gd->bg_free_blocks_count);
  printf(" bg_free_inodes_count: %d\n", gd->bg_free_inodes_count);
  printf(" bg_used_dirs_count: %d\n", gd->bg_used_dirs_count);
  /* printf("bg_pad: %d\n", gd->bg_pad);
  printf("bg_reserved[3]: %d\n", gd->bg_reserved[3]); */
  printf("---------------------------\n");
}

static void ReadINode(int fd, int inode_num, struct ext2_inode *in, struct ext2_group_desc *gd)
{

  lseek(fd, (gd->bg_inode_table) * 4096 + (inode_num - 1) * sizeof(struct ext2_inode), SEEK_SET);
  read(fd, in, sizeof(struct ext2_inode));

}

static void ReadDirectory(int fd, struct ext2_dir_entry_2 *de, struct ext2_inode *in)
{
  printf("i_block[0]: %d\n",(in->i_block[0] - 1) );
  lseek(fd, (in->i_block[0]) * 4096, SEEK_SET);
  read(fd, de, sizeof(*de));
}
static void PrintRootDirectory(int fd, struct ext2_inode *in, struct ext2_group_desc *gd)
{
  int i = 0;
  char file_name[1000];
  int read_count = 0;
  struct ext2_inode dest_inode;
  struct ext2_dir_entry_2 dir_ent;
  struct ext2_dir_entry_2 *de = &dir_ent;
  ReadDirectory(fd, de, in);

  while (in->i_blocks > i)
  {
    memcpy(file_name, de->name, de->name_len);
    file_name[de->name_len] = '\0';
    printf("inode num: %d\n", de->inode);
    printf("inode type: %d\n",de->file_type);
    if(1 == de->file_type)
    {
      ReadINode(fd,de->inode,&dest_inode,gd);
      PrintInode(fd,&dest_inode);
      
    }

    printf("name is: %s\n\n",file_name);
    de = (void*) de + de->rec_len;
    
    ++i;
  }
}
static void PrintInode(int fd, struct ext2_inode *in)
{

    size_t i = 0;
    unsigned char mem_block[4096];
    unsigned int num_blocks = in->i_blocks / (2 << 2);

    printf("--num blocks %d\n", num_blocks);
    puts("Content:");
    for (i = 0; num_blocks > i; ++i)
    {
      lseek(fd,  (in->i_block[0]) * 4096, SEEK_SET);
      read(fd, mem_block, 4096);
      printf("%s\n", mem_block);
    }
  
}
static void  PrintFileList(const char *path)
{

  DIR *d;
  struct dirent *dir;
  d = opendir(path);
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
}
static void PrintFileContent(const char *file_path)
{
  FILE *file_pointer;
  char function_line[1000];

  file_pointer = fopen(file_path,"r");
  printf("file path content \n");

  while (fgets(function_line, 1024, file_pointer))
  {
    printf("%s",function_line);
  }

  fclose(file_pointer);
  printf("---------------------------\n");
}
