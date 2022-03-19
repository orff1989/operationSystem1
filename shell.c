#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

void remove_first_n_chars(char* str, int n){
    int j=0;
    for (size_t i = n; i < strlen(str)+2; i++)
        str[j++]=str[i];
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
            // printf("tcp porttt");
        }

        else if(strncmp(buff, "DIR", 3)==0){
            list_of_files(cwd);
            printf("%s\n",cwd);
        }

        else if(strncmp(buff, "CD", 2)==0){
            remove_first_n_chars(buff, 3);
            if(chdir(buff)<0) printf("could not change to %s",buff);
        }
        else system(buff);
        

        
    }
}