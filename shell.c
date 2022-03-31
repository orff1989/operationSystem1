#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<errno.h>
#include "client.c"
#include "server.c"

int stdoutt=1;
int file;
char ans[16];


void remove_first_n_chars(char* str, int n){
    int j=0;
    for (size_t i = n; i < strlen(str)+2; i++)
        str[j++]=str[i];
}

char* getWordNumber(char* str1, int num){
    int k=0;
    char ch;
    strcpy(ans,"");
    for (size_t i = 0; i < strlen(str1); i++)
    {
        ch =str1[i];
        if(ch ==' ') k++;

        if(k==num && ch!=' ') strncat(ans,&ch,1);
    }
    return ans;
    
}

void list_of_files(char* str){
   struct dirent* de;  
  
    DIR* d = opendir(".");
    if (d == NULL)  return;
    
    while ((de = readdir(d)) != NULL){
            strcat(str,de->d_name);
            strcat(str,", ");
    }

    closedir(d);    
}

void takeInput(){
    char buff[1024];
    char cwd[256];

    printf("Yes Master?\n");
    
    while(1){

        // getting the path of out directory
        getcwd(cwd, sizeof(cwd)); 
        printf("%s ", cwd);
        
        // getting the command
        fgets(buff, sizeof(buff), stdin);
        if(buff[strlen(buff)-1]='\n') buff[strlen(buff)-1]='\0'; 
        
        // exits from the shell
        if (strncmp(buff, "EXIT",4)==0) return;

        // printing echo command
        else if(strncmp(buff, "echo", 4)==0){
            remove_first_n_chars(buff, 5);
            printf("%s\n",buff);
        }

        else if(strncmp(buff, "TCP PORT", 8)==0){
            FILE * fPtr = fopen("tt.txt", "w");
            fclose(fPtr);
            file = open("tt.txt",O_WRONLY | O_APPEND);
            dup2(file, 1); 
           
            printf("got TCP PORT....\n");
            
              
        }
        else if(strncmp(buff, "LOCAL", 5)==0){
            dup2(1, file); 
            close(file);
            fflush(stdout);
        }

        else if(strncmp(buff, "DIR", 3)==0){
            strcpy(cwd,"");
            list_of_files(cwd);
            printf("%s\n",cwd);
        }

        else if(strncmp(buff, "CD", 2)==0){
            remove_first_n_chars(buff, 3);
            if(chdir(buff)<0) printf("could not change to %s",buff);
            //chdir is system call.
        }

        else if(strncmp(buff, "DELETE", 6)==0){
            char fileName[24]="";
            strcpy(fileName, getWordNumber(buff,1));
            if (unlink(fileName) != 0)
            perror("unlink() error");
            //yes, we used both system call(unlink function) and libary functions
        }

        else if(strncmp(buff, "COPY", 4)==0){
            
            char ch, source_file[20]="", target_file[20]="";
            FILE *source, *target;

            strcpy(source_file,getWordNumber(buff,1));
            strcpy(target_file,getWordNumber(buff,2));
            
            source = fopen(source_file, "r");
            if( source == NULL )
            {
                printf("cant open file: %s\n", source_file);
                exit(EXIT_FAILURE);
            }
            target = fopen(target_file, "w");
 
            if( target == NULL ){
                fclose(source);
                printf("cant open new file\n");
                exit(EXIT_FAILURE);
            }
            
            while( ( ch = fgetc(source) ) != EOF )
                fputc(ch, target);
            
            
            fclose(source);
            fclose(target);
            // i did not use system call in coping file
        }
        else{ 
        //system(buff); //system function is not system call

        int state;
        pid_t p;

        if (strlen(buff)==0)
        return(1);

        if ((p = fork()) < 0){
        state = -1;
        }
        else if (p == 0){
        execl("/bin/sh", "sh", "-c", buff, (char *)0);
        _exit(127); 
        }
        else
        while (waitpid(p, &state, 0) < 0)
        {
        if (errno != EINTR)
        state = -1; 
        break;
        }
        
        }
        
    }
}