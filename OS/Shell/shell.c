#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
int num_of_args(const char* cmdline)
{
    int i=0,k=1;
    for (i=0;i<strlen(cmdline);i++)
    {
        if(cmdline[i]==' ') {k++;}
    }
    return k;
}
int largest_arg(const char* buffer)
{
    int i=0;
    int k = 0;
    int maximum;
    int test = num_of_args(buffer)*sizeof(int);
    int* arr=malloc(test);
    for (i = 0; i<num_of_args(buffer); i++)
    {
        arr[i]=0;
    }
    for (i=0;i<strlen(buffer);i++)
    {
        if (buffer[i]!=' ') 
        {
            arr[k]++;
        }
        else
        {
            k++;
            i++;
        }
    }
    
    for (k=0;k<num_of_args(buffer);k++)
    {
        if(arr[k]>maximum)
        maximum  = arr[k];
    }
    free(arr);
  return maximum;
}
char** parse_cmdline(const char* cmdline)
{
    char* temp = strdup(cmdline);
    char** array = malloc(100 * sizeof(char*));
    int k = num_of_args(cmdline);
    array = realloc(array,k*sizeof(char*));
    int i;
    for (i=0;i<k;i++)
    {
        array[i]= malloc(sizeof(char)*largest_arg(cmdline));
    }
    i=0;
    array[i] = strtok(temp," ");   
    while(array[i]!=NULL)
    {
        array[++i] = strtok(NULL," ");
    }
    array[i-1][strlen(array[i-1])-1] = '\0';
    return array;
    free(array);
}

int main(int argc, char **argv)
{

        char** array;
        int status;
        int i;
        char buffer[1024];
        while(1)
        {
            printf("shell:");
            fgets(buffer, 1024, stdin);
            int k = num_of_args(buffer);
            array = malloc(k*sizeof(char*));
            for (i=0;i<k;i++)
            {
                array[i]= malloc(sizeof(char)*largest_arg(buffer));
            }
            largest_arg(buffer);
            array = parse_cmdline(buffer);
            pid_t pid = fork(); 
            if(pid<0)
            {
                printf("Fork failed:\n");
                status = -1;
            }
            else if(pid==0)
            {          
                    int rv = execv(array[0], array);
                    if (rv<0 && array[0][0]!='\0' ) perror(array[0]);

            }
            else {
                if (waitpid(pid,&status,0)!=pid)
                {
                    status = -1;
                }
            }
            free(array);
        }
        return status;
}