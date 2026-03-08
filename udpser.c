#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server, client;
    char buffer[1024];
    socklen_t len = sizeof(client);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address configuration
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bind the socket to the server address
    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Waiting for data...\n");

    // Receive data from client
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &len);
    printf("Message from client: %s\n", buffer);

    // Respond to client
    sendto(sockfd, "Hello from server", 18, 0, (struct sockaddr *)&client, len);

    close(sockfd);
    return 0;
}