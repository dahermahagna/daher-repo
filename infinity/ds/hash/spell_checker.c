#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "hash.h"
#include <string.h>

#define INDEX_LENGH (65)

static void FindWord(   char * word_to_find);
static char *GetDictionary(const char *fname);
size_t HashCodeDic(const void *data);
int CmpFuncDic(const void *data, const void *data_to_compare);
int PrintWord(void *data, void *param);

int main(int argc, char *argv[])
{
    (void) argc;
    printf("%s\n",argv[1]);
    FindWord(argv[1]);
    return 0;
}

static void FindWord(char *word_to_find)
{

    hash_table_ty *dic_htable = HashCreate(HashCodeDic, CmpFuncDic, INDEX_LENGH);
    char *dic_add = GetDictionary("american-english");
    char *word = NULL;
    size_t count = 0;

    word = strtok(dic_add, "\n");

    while (NULL != word)
    {

        HashInsert(dic_htable, (void *)word);
        word = strtok(NULL, "\n");
        ++count;
    }

    word = HashFind(dic_htable, (void *)word_to_find);
    if(NULL == word)
    {
        printf("the word is not found");
    }
    else
    {
        printf("the word \"%s\" is found",word);
    }
}

static char *GetDictionary(const char *fname)
{
    unsigned char *result;
    unsigned int len;
    struct stat buf;
    int fd = open(fname, O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "Error: Unable to read dictionary file %s\n", fname);
        return (char *)0;
    }

    if (fstat(fd, &buf) < 0)
    {
        fprintf(stderr, "Error: Unable to determine file size\n");
        return (char *)0;
    }

    len = (unsigned int)buf.st_size;
    result = (unsigned char *)mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (result == MAP_FAILED)
    {
        fprintf(stderr, "Error: Unable to memory map dictionary!\n");
    }
    return (char *)result;
}

size_t HashCodeDic(const void *data)
{
    return (size_t)(*(char *)data) % 65;
}
int CmpFuncDic(const void *data, const void *data_to_compare)
{
    return !strcmp((char *)data, (char *)data_to_compare);
}
int PrintWord(void *data, void *param)
{
    printf("\n%d)the word is: %s\n", *(int *)param, (char *)data);
    ++*(int *)param;
    return 0;
}
