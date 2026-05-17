
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 256

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);
    char buffer[BUF_SIZE];

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(sockfd, 1) < 0) {
        perror("listen");
        return 1;
    }

    printf("Server waiting on port 8080...\n");

    int clientfd = accept(sockfd, (struct sockaddr *)&client, &client_len);
    if (clientfd < 0) {
        perror("accept");
        close(sockfd);
        return 1;
    }

    // Greeting
    recv(clientfd, buffer, BUF_SIZE, 0);
    printf("Client: %s\n", buffer);
    strcpy(buffer, "220 127.0.0.1");
    send(clientfd, buffer, strlen(buffer)+1, 0);

    // HELO
    recv(clientfd, buffer, BUF_SIZE, 0);
    printf("Client: %s\n", buffer);
    send(clientfd, "250 ok", 7, 0);

    // MAIL FROM
    recv(clientfd, buffer, BUF_SIZE, 0);
    printf("Client: %s\n", buffer);
    send(clientfd, "250 ok", 7, 0);

    // RCPT TO
    recv(clientfd, buffer, BUF_SIZE, 0);
    printf("Client: %s\n", buffer);
    send(clientfd, "250 ok", 7, 0);

    // DATA
    recv(clientfd, buffer, BUF_SIZE, 0);
    printf("Client: %s\n", buffer);
    send(clientfd, "354 Go ahead", 13, 0);

    // Mail body
    recv(clientfd, buffer, BUF_SIZE, 0);
    printf("Mail body: %s\n", buffer);

    // QUIT
    recv(clientfd, buffer, BUF_SIZE, 0);
    printf("Client: %s\n", buffer);
    send(clientfd, "221 OK", 7, 0);

    close(clientfd);
    close(sockfd);
    return 0;
}