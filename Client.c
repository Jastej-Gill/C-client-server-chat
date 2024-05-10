#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#define SERVER_IP "172.23.179.14"
#define PORT 5050
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0
        },
        {
            client_fd,
            POLLIN,
            0
        }
    };
    
    while (1) {
        
        poll(fds, 2, 50000);
        

        memset(buffer, 0, BUFFER_SIZE);        

        if(fds[1].revents & POLLIN){
            printf("\n");
            if (recv(client_fd, buffer, BUFFER_SIZE, 0) == 0){
                return 0;
            }

            else if(strcmp(buffer, "exit\n") == 0){
                break;
            }
            printf("Server: %s\n", buffer);
        }
        
        if(fds[0].revents & POLLIN){        
            fgets(buffer, BUFFER_SIZE, stdin);                 
            send(client_fd, buffer, strlen(buffer), 0);


      
        }
        if(strcmp(buffer, "exit\n") == 0){
                break;
        } 

        // printf("Client: ");
        // fgets(buffer, BUFFER_SIZE, stdin);
        // send(client_fd, buffer, strlen(buffer), 0);
        // memset(buffer, 0, BUFFER_SIZE);
        // read(client_fd, buffer, BUFFER_SIZE);
        // printf("Server: %s", buffer);
    }
    return 0;
}