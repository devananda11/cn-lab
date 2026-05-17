#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define WINDOW 4
#define TOTAL 10
#define TIMEOUT 3

int main() {
    int sockfd;
    int sent[TOTAL + 1] = {0};
    int acked[TOTAL + 1] = {0};
    int base = 1, i, ack;

    struct sockaddr_in server;
    struct timeval tv;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Server details
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Connected to server.\n\n");

    // Set timeout
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (base <= TOTAL) {

        // Send all unacknowledged frames in the window
        for (i = base; i < base + WINDOW && i <= TOTAL; i++) {
            if (!acked[i]) {
                printf("Sending frame %d\n", i);
                sprintf(buffer, "%d", i);
                send(sockfd, buffer, strlen(buffer) + 1, 0);
                sent[i] = 1;
            }
        }

        // Receive one ACK
        int n = read(sockfd, buffer, sizeof(buffer) - 1);

        if (n > 0) {
            buffer[n] = '\0';
            ack = atoi(buffer);

            printf("ACK received for frame %d\n\n", ack);
            acked[ack] = 1;
        } else {
            printf("Timeout! Resending unacknowledged frames.\n\n");
        }

        // Slide window
        while (base <= TOTAL && acked[base]) {
            base++;
        }
    }

    printf("All frames sent successfully.\n");

    close(sockfd);
    return 0;
}