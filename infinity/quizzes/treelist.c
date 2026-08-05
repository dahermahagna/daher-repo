
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h> /*strcat , strcpy*/
#include <stdio.h>  /*printf*/

/*-------------------------*/
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* #include <sys/mount.h> */
#include <assert.h>
#include <dirent.h>

void PrintDirectoryList(char *path, size_t deep)
{
    DIR *dirstream;
    struct dirent *directory;
    char child_path[1024] = {0};
    size_t i = 0;
    dirstream = opendir(path);
    if (dirstream)
    {
        while ((directory = readdir(dirstream)) != NULL)
        {
            if (strcmp(directory->d_name, ".") != 0 && strcmp(directory->d_name, "..") != 0)
            {

                if (directory->d_type == 4)
                {

                    printf("├─");
                    for (i = 0; i < deep; i++)
                    {
                        printf("──");
                    }
                    printf("\033[0;34m%s\033[0m\n", directory->d_name);
                    strcpy(child_path, path);
                    strcat(child_path, "/");
                    strcat(child_path, directory->d_name);
                    PrintDirectoryList(child_path, deep + 1);
                }
                else
                {
                printf("│");
                for (i = 0; i < deep; i++)
                {
                    printf("   ");
                }
                    printf("%s\n", directory->d_name);
                    
                }
                
            }
        }
    }
}

int main()
{
    char path[256] = "/home/daher/git/cpp_fs";
    PrintDirectoryList(path, 0);
    return 0;
}