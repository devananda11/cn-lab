#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd, newsock;
    struct sockaddr_in server;
    int addrlen = sizeof(server);
    char buffer[BUFFER_SIZE] = {0};
    int ack_prob = 70;  // 70% chance of sending ACK

    srand(time(0));  // Random seed for ACK simulation

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: Waiting for connection...\n");

    // Accept connection from client
    if ((newsock = accept(sockfd, (struct sockaddr *)&server, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: Connection established.\n");

    while (1) {
        // Receive packet from client
        int valread = read(newsock, buffer, BUFFER_SIZE);
        if (valread == 0) break;

        printf("Server: Received packet - %s\n", buffer);

        // Simulate ACK or loss
        if (rand() % 100 < ack_prob) {
            printf("Server: ACK sent for packet %s\n\n", buffer);
            send(newsock, "ACK", strlen("ACK"), 0);
        } else {
            printf("Server: ACK lost for packet %s\n\n", buffer);
        }

        memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
    }

    close(newsock);
    close(sockfd);
    return 0;
}