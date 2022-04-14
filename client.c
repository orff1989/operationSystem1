#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void CClient()
{   
    int socket_desc;
    char ch;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    
    // Clean buffers:
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Unable to create socket\n");
        return -1;
    }
    
    
    
    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Send connection request to server:
    if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect\n");
        return -1;
    }
    strcpy(client_message,"");
    FILE* ptr = fopen("tt.txt","r");
    do {
        ch = fgetc(ptr);
        strncat(client_message, &ch, 1);
 
        // Checking if character is not EOF.
        // If it is EOF stop eading.
    } while (ch != EOF);
    
    // Send the message to server:
    if(send(socket_desc, client_message, strlen(client_message), 0) < 0){
        printf("Unable to send message\n");
        return -1;
    }
    strcpy(client_message,"");
    
    
    // Close the socket:
    close(socket_desc);
    
    
}

void systemFunc(char* buff){
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

        void copyFunction(char* sFile, char* tFile){
            FILE *s, *t;
            char ch;
            
            s = fopen(sFile, "r");
            if( s == NULL )
            {
                printf("cant open file: %s\n", sFile);
                exit(EXIT_FAILURE);
            }
            t = fopen(tFile, "w");
 
            if( t == NULL ){
                fclose(s);
                printf("cant open new file\n");
                exit(EXIT_FAILURE);
            }
            
            while( ( ch = fgetc(s) ) != EOF )
                fputc(ch, t);
            
            
            fclose(s);
            fclose(t);
            // i did not use system call in coping file
        
}
