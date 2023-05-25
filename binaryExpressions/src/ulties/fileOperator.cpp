#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

static char *pfilecontent = NULL;

char *foRead(char *filePath)
{
    int ret = -1;
    FILE *fstream = NULL;
    char *pnewfilecontent = NULL;
    try
    {
        struct stat file_info;
        ret = stat(filePath, &file_info);
        if (ret == 0)
        {
            pnewfilecontent = (char *)realloc(pfilecontent, file_info.st_size+2);
            if (pnewfilecontent)
                pfilecontent = pnewfilecontent;
            else
            {
                free(pfilecontent);
                pfilecontent = NULL;
            }
            fstream = fopen(filePath, "rt");
            if (fstream && pfilecontent){
                ret = fscanf(fstream, "%s", (pfilecontent+1));
                *pfilecontent = '(';
                int strlens = strlen(pfilecontent);
                *(pfilecontent+strlens)=')';
            }
            if (fstream)
            {
                fclose(fstream);
                fstream = NULL;
            }
        }
        else
        {
            int err = errno;
            switch (err)
            {
            case ENOENT:
                printf("参数file_name指定的文件不存在\n");
                break;
            case ENOTDIR:
                printf("路径中的目录存在但却非真正的目录\n");
                break;
            case ELOOP:
                printf("欲打开的文件有过多符号连接问题，上限为16符号连接\n");
                break;
            case EFAULT:
                printf("参数buf为无效指针，指向无法存在的内存空间\n");
                break;
            case ENOMEM:
                printf("核心内存不足\n");
                break;
            case ENAMETOOLONG:
                printf("参数file_name的路径名称太长\n");
                break;
            }
        }
    }
    catch (const void *e)
    {
        printf("Unkonw error ocurrs \n");
        return pfilecontent;
    }
    return pfilecontent;
}
void foClose()
{
    if (pfilecontent)
    {
        free(pfilecontent);
        pfilecontent = NULL;
    }
}
/**
 * @brief 写内容到指定文件
 * 
 * @param filepath 文件路径
 * @param pContent 要写入文件的内容
 */
void foWrite(char* filePath,char* pContent){
    if(filePath&&pContent){
        FILE *fstream = fopen(filePath, "wt");
        if(fstream){
            fprintf(fstream,"%s",pContent);
            fclose(fstream);
        }
    }
}