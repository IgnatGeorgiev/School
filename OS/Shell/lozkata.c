#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h> 
#include <fcntl.h> 
#include <unistd.h> 


char** parse_cmdline(char* str){
	char **array = malloc(100*sizeof(char*));
    int i = 0;
    array[i] = strtok(str," ");  
    while(array[i]!=NULL){
        array[++i] = strtok(NULL," ");
    }

    array[i - 1][strlen(array[i-1]) - 1] = '\0';

    return array;
}

int process_maker(char* filename,char** result_execv){
		char err_string[100];
		int status;
		pid_t pid;
		pid = fork(); //-----------------------Makes a child process

		if(pid < 0){//-------------------------Failure
			printf("Error executing!\n");
			status = -1;
		}else if(pid == 0){//------------------Child process
				int executable_error_check = execv(filename, result_execv);
				snprintf(err_string,sizeof(err_string),"'%s'",result_execv[0]);
				if(executable_error_check < 0 && result_execv[0][0] != '\0'){
					result_execv[0][strlen(result_execv[0] - 1)] = '\0';
					perror(err_string);
				}
		}else{//-------------------------------Child process finished
			if(waitpid(pid,&status,0) != pid){
				status = -1;
			}
		}
		return status;

}

void user_input(){
	char buff[10];
	char *temp = (char*)malloc(100000*sizeof(char*));
	int counter = 0;
	int i,status_read = 1;
	char** splitted_array = malloc(100*sizeof(char*));

	while((status_read = read(STDIN_FILENO,buff,1))>0){	//Saving every char from the input in temp
		temp[counter] = buff[0];	
		counter++;		
		if(buff[0] == '\n'){
			splitted_array = malloc(100*sizeof(char*));
			splitted_array = parse_cmdline(temp);

			process_maker(splitted_array[0],splitted_array);
			temp = NULL;
			temp = malloc(100000*sizeof(char));
			counter = 0;
		}
	}

}

int main(){
	user_input();
	return 0;
}