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

    struct sockaddr_in server;
    char buffer[BUF_SIZE];

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    // Greeting
    strcpy(buffer, "hi");
    send(sockfd, buffer, strlen(buffer)+1, 0);
    recv(sockfd, buffer, BUF_SIZE, 0);
    printf("Server: %s\n", buffer);

    // HELO
    strcpy(buffer, "HELO 127.0.0.1");
    send(sockfd, buffer, strlen(buffer)+1, 0);
    recv(sockfd, buffer, BUF_SIZE, 0);
    printf("Server: %s\n", buffer);

    // MAIL FROM
    printf("Enter FROM address: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    char mail_from[BUF_SIZE];
    snprintf(mail_from, BUF_SIZE, "MAIL FROM %s", buffer);
    send(sockfd, mail_from, strlen(mail_from)+1, 0);
    recv(sockfd, buffer, BUF_SIZE, 0);
    printf("Server: %s\n", buffer);

    // RCPT TO
    printf("Enter TO address: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    char rcpt_to[BUF_SIZE];
    snprintf(rcpt_to, BUF_SIZE, "RCPT TO %s", buffer);
    send(sockfd, rcpt_to, strlen(rcpt_to)+1, 0);
    recv(sockfd, buffer, BUF_SIZE, 0);
    printf("Server: %s\n", buffer);

    // DATA
    strcpy(buffer, "DATA");
    send(sockfd, buffer, strlen(buffer)+1, 0);
    recv(sockfd, buffer, BUF_SIZE, 0);
    printf("Server: %s\n", buffer);

    // Mail body
    printf("Enter mail body (end with $): ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "$\n")] = 0; // Remove trailing $ or newline
    char mail_body[BUF_SIZE];
    snprintf(mail_body, BUF_SIZE, "%s", buffer);
    send(sockfd, mail_body, strlen(mail_body)+1, 0);

    // QUIT
    strcpy(buffer, "QUIT");
    send(sockfd, buffer, strlen(buffer)+1, 0);
    recv(sockfd, buffer, BUF_SIZE, 0);
    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;
}