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

    struct sockaddr_in server;
    socklen_t server_len = sizeof(server);
    char buffer[BUF_SIZE];

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);

    // Greeting
    strcpy(buffer, "hi");
    sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&server, server_len);
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server, &server_len);
    printf("Server: %s\n", buffer);

    // HELO
    strcpy(buffer, "HELO 127.0.0.1");
    sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&server, server_len);
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server, &server_len);
    printf("Server: %s\n", buffer);

    // MAIL FROM
    printf("Enter FROM address: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    char mail_from[BUF_SIZE];
    snprintf(mail_from, BUF_SIZE, "MAIL FROM %s", buffer);
    sendto(sockfd, mail_from, strlen(mail_from)+1, 0, (struct sockaddr *)&server, server_len);
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server, &server_len);
    printf("Server: %s\n", buffer);

    // RCPT TO
    printf("Enter TO address: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    char rcpt_to[BUF_SIZE];
    snprintf(rcpt_to, BUF_SIZE, "RCPT TO %s", buffer);
    sendto(sockfd, rcpt_to, strlen(rcpt_to)+1, 0, (struct sockaddr *)&server, server_len);
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server, &server_len);
    printf("Server: %s\n", buffer);

    // DATA
    strcpy(buffer, "DATA");
    sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&server, server_len);
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server, &server_len);
    printf("Server: %s\n", buffer);

    // Mail body
    printf("Enter mail body (end with $): ");
    char mail_body[BUF_SIZE];
    int i = 0, c;
    while ((c = getchar()) != '$' && i < BUF_SIZE-1) {
        mail_body[i++] = c;
    }
    mail_body[i] = '\0';
    sendto(sockfd, mail_body, strlen(mail_body)+1, 0, (struct sockaddr *)&server, server_len);

    // QUIT
    strcpy(buffer, "QUIT");
    sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&server, server_len);
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server, &server_len);
    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;
}