#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include<stdlib.h>
#include <pthread.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<errno.h>
#include "client.c"
#include "server.c"

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
    FILE *fl;
    int nf;
    int stdout_copy;

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
        else if(strncmp(buff, "ECHO", 4)==0){
            remove_first_n_chars(buff, 5);
            printf("%s\n",buff);
        }

        else if(strncmp(buff, "TCP PORT", 8)==0){
            pthread_t thread_id;
            
            printf("got TCP PORT....\n");
            fl = fopen("tt.txt","w");
            nf = fileno(fl);
            stdout_copy = dup(STDOUT_FILENO);
            dup2(nf, STDOUT_FILENO);
            pthread_create(&thread_id, NULL, SServer, NULL);

            sleep(1);
            CClient();
        
            
        }
        else if(strncmp(buff, "LOCAL", 5)==0){
            fflush(stdout);
            dup2(stdout_copy, STDOUT_FILENO);
            close(stdout_copy);
            close(nf);
            close(fl);
            puts("hellp2");
            
        
        }

        else if(strncmp(buff, "DIR", 3)==0){
            strcpy(cwd,"");
            list_of_files(cwd);
            printf("%s\n",cwd);
        }

        else if(strncmp(buff, "CD", 2)==0){
            remove_first_n_chars(buff, 3);
            if(chdir(buff)<0) printf("could not change to %s\n",buff);
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
            char ch, sFile[35]="", tFile[35]="";
           
            strcpy(sFile,getWordNumber(buff,1));
            strcpy(tFile,getWordNumber(buff,2));

            copyFunction(sFile, tFile);

        }
        else{ 
        //system(buff); //system function is not system call
        systemFunc(buff);
        }
        
    }
}

int main()
{
    takeInput();
    return 0;
}
