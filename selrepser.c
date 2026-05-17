#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define LOSS 30      // 30% chance of ACK loss

int main() {
    int sockfd, newsock, frame;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    char buffer[1024];

    srand(time(0));

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind and listen
    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    listen(sockfd, 5);

    printf("Waiting for connection...\n");

    // Accept connection
    newsock = accept(sockfd, (struct sockaddr *)&client, &len);

    printf("Client connected.\n\n");

    while (1) {
        int n = read(newsock, buffer, sizeof(buffer) - 1);

        if (n <= 0)
            break;

        buffer[n] = '\0';
        frame = atoi(buffer);

        printf("Received frame %d\n", frame);

        // Simulate ACK loss
        if (rand() % 100 < LOSS) {
            printf("ACK %d lost!\n\n", frame);
        } else {
            printf("Sending ACK %d\n\n", frame);

            sprintf(buffer, "%d", frame);
            send(newsock, buffer, strlen(buffer) + 1, 0);
        }
    }

    close(newsock);
    close(sockfd);
    return 0;
}