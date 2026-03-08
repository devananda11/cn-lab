#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 256

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

    printf("Server waiting on port 8080...\n");

    // Greeting
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client, &client_len);
    printf("Client: %s\n", buffer);
    strcpy(buffer, "220 127.0.0.1");
    sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&client, client_len);

    // HELO
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client, &client_len);
    printf("Client: %s\n", buffer);
    sendto(sockfd, "250 ok", 7, 0, (struct sockaddr *)&client, client_len);

    // MAIL FROM
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client, &client_len);
    printf("Client: %s\n", buffer);
    sendto(sockfd, "250 ok", 7, 0, (struct sockaddr *)&client, client_len);

    // RCPT TO
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client, &client_len);
    printf("Client: %s\n", buffer);
    sendto(sockfd, "250 ok", 7, 0, (struct sockaddr *)&client, client_len);

    // DATA
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client, &client_len);
    printf("Client: %s\n", buffer);
    sendto(sockfd, "354 Go ahead", 13, 0, (struct sockaddr *)&client, client_len);

    // Mail body
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client, &client_len);
    printf("Mail body: %s\n", buffer);

    // QUIT
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client, &client_len);
    printf("Client: %s\n", buffer);
    sendto(sockfd, "221 OK", 7, 0, (struct sockaddr *)&client, client_len);

    close(sockfd);
    return 0;
}