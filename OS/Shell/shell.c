//--------------------------------------------
// NAME: Ignat Georgiev
// CLASS: XIa
// NUMBER: 14
// PROBLEM: #2
// FILE NAME: shell.c
// FILE PURPOSE:
// Реализация на shell
// При въвеждане на команда, програмата я интерпретира
//---------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
//--------------------------------------------
// FUNCTION: num_of_args 
// Тази функция намира броя на аргументите на командата, въведена от потребителя
// PARAMETERS:
// Единствен параметър : const char* cmdline - въведеният от потребителя стринг, в който намираме спейсовете и от там намираме броя на аргументите
//----------------------------------------------
int num_of_args(const char* cmdline)
{
    int i=0,k=1;
    for (i=0;i<strlen(cmdline);i++)
    {
        if(cmdline[i]==' ') {k++;}
    }
    return k;
}
//--------------------------------------------
// FUNCTION: num_of_args 
// Тази функция намира най-големия аргумент на командата, въведена от потребителя
// PARAMETERS:
// Единствен параметър : const char* cmdline - въведеният от потребителя стринг, в който намираме дължитата на аргументите и  връщаме дължината на най-дългия аргумент
//----------------------------------------------
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
        if (buffer[i]!=' ') //проверка дали все още символът не е спейс и ако не е увеличаваме стойността на елемента от масива с едно
        {
            arr[k]++;
        }
        else//сменяме на следващ елемент от масива
        {
            k++;
            i++;
        }
    }
    //в следващия цикъл търсим най-голямото число в масив
    for (k=0;k<=num_of_args(buffer);k++)
    {
        if(arr[k]>maximum)
        maximum  = arr[k];
    }
    printf("%d\n",maximum);

    free(arr);//освобождаваме паметта заделена за масива
  return maximum;
}
//--------------------------------------------
// FUNCTION: parse_cmdline
// Функция за обработване на командния ред, така че да го използваме с execv
// PARAMETERS:
// Единствен параметър : const char* cmdline -Стринг,взет от стандартния вход - използваме го, за да реализираме масив от стрингове, като разделим първоначалния стринг чрез strtok
//----------------------------------------------
char** parse_cmdline(const char* cmdline)
{
    char* temp = strdup(cmdline);//Временна променлива от тип стринг, използваме я, за да не използваме const char* във strtok
    
    int k = num_of_args(cmdline);//намираме броя на аргументите за да реалокираме нужната памет
    int i;
    char** array = malloc(k * sizeof(char*));//алокираме памет за масив от стрингове
    for (i=0;i<k;i++)
    {
        array[i]= malloc(sizeof(char*)*largest_arg(cmdline));//алокираме памет за всеки един стринг, като размер използваме най-големия аргумент
    }
    i=0;
    array[i] = strtok(temp," ");//взимаме първия аргумент от стринга и го записваме като първи елемент в масива
    while(array[i]!=NULL)
    {
        array[++i] = strtok(NULL," ");//взимаме следващите аргументи и ги записваме в масива
    }
    array[i-1][strlen(array[i-1])] = '\0';//слагаме терминираща нула на края на последния елемент от масива
    return array;
}

char* readingInput()
{
  int cap = 4096; /* Initial capacity for the char buffer */
  int len = 0; /* Current offset of the buffer */
  char *buffer = malloc(cap * sizeof (char));
  int c;

  /* Read char by char, breaking if we reach EOF or a newline */
  while ((c = fgetc(stdin)) != '\n' && !feof(stdin))
    {
      buffer[len] = c;

      /* When cap == len, we need to resize the buffer
       * so that we don't overwrite any bytes
       */
      if (++len == cap)
        /* Make the output buffer twice its current size */
        buffer = realloc(buffer, (cap *= 2) * sizeof (char));
    }

  /* Trim off any unused bytes from the buffer */
  buffer = realloc(buffer, (len + 1) * sizeof (char));
  /* Pad the last byte so we don't overread the buffer in the future */
  buffer[len] = '\0';
  return buffer;
}
int main(int argc, char **argv)
    {
    char** array;//инициализираме масив от стрингове, нужен ни за изпълнението на execv
    int status;
    int i;
    char* buffer;
    while(1) //безкраен цикъл, продължава да се изпълнява докъто в терминала не се подаде ctrl + C
    {
        printf("shell:");
        buffer = readingInput(); 
        int k = num_of_args(buffer);//намираме броя на аргументите
        array = parse_cmdline(buffer);//извикваме функцията, която ни обработва командния ред
        pid_t pid = fork();//създаваме нов процес и запаметяваме ID-то му в променлива от тип pid_t
        if(pid<0)//Ако id-то на процеса е по-малък от 0 има грешка в създаването на процеса
        {
            printf("Fork failed:\n");
            status = -1;
        }
        else if(pid==0)//Ако id-to на процеса е = 0, то тогава можем свободно да изпълним програмата
        {          
                int rv = execv(array[0], array);
                if (rv<0 && array[0][0]!='\0' ) perror(array[0]);

        }
        else {
            if (waitpid(pid,&status,0)!=pid)//следим състоянието на процеса,използвайки неговия процесен идентификатор
            {
                status = -1;
            }
        }
    }
    return status;
}