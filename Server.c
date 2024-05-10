#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#define PORT 5050
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd, addr_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);

    // stdin - 0
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
 
        // Receive a message from the client
        memset(buffer, 0, BUFFER_SIZE);
        // read(client_fd, buffer, BUFFER_SIZE);
        // printf("Client: %s", buffer);
    
        // // Send a message to the client
        // fgets(buffer, BUFFER_SIZE, stdin);
        // send(client_fd, buffer, strlen(buffer), 0);
        if (fds[0].revents & POLLIN){     
            fgets(buffer, BUFFER_SIZE, stdin); 

            if(strcmp(buffer, "exit\n") == 0){
                send(client_fd, buffer, strlen(buffer), 0); 
            }

            else{
                send(client_fd, buffer, strlen(buffer), 0); 

            }
      
        }

        else if(fds[1].revents & POLLIN) {
            if(recv(client_fd, buffer, BUFFER_SIZE, 0) == 0){
                return 0;
            }
            printf("\n");
            printf("Client: ");
            printf("%s\n", buffer);
            
        }




    }

    return 0;
}