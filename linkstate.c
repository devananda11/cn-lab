#include <stdio.h>

#define MAX 10
#define INF 9999

void dijkstra(int graph[MAX][MAX], int n, int start) {
    int dist[MAX], visited[MAX], nextHop[MAX];
    int i, j, min, next;

    // Initialize arrays
    for (i = 0; i < n; i++) {
        if (graph[start][i] == 0)
            dist[i] = INF;
        else
            dist[i] = graph[start][i];

        visited[i] = 0;

        if (i != start && dist[i] != INF)
            nextHop[i] = i;      // Direct neighbor
        else
            nextHop[i] = -1;
    }

    dist[start] = 0;
    visited[start] = 1;

    // Dijkstra's algorithm
    for (i = 1; i < n; i++) {
        min = INF;
        next = -1;

        // Find nearest unvisited node
        for (j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < min) {
                min = dist[j];
                next = j;
            }
        }

        if (next == -1)
            break;

        visited[next] = 1;

        // Update distances
        for (j = 0; j < n; j++) {
            if (!visited[j] && graph[next][j] != 0) {
                if (dist[next] + graph[next][j] < dist[j]) {
                    dist[j] = dist[next] + graph[next][j];
                    nextHop[j] = nextHop[next];
                }
            }
        }
    }+

    // Print routing table
    printf("\nRouting Table for Router %d\n", start + 1);
    printf("Destination\tCost\tNext Hop\n");

    for (i = 0; i < n; i++) {
        if (i != start) {
            printf("%d\t\t", i + 1);

            if (dist[i] == INF)
                printf("INF\tNo route\n");
            else
                printf("%d\t%d\n", dist[i], nextHop[i] + 1);
        }
    }
}

int main() {
    int graph[MAX][MAX];
    int n, i, j, start;

    printf("Enter number of routers: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (0 for no link):\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    printf("Enter source router: ");
    scanf("%d", &start);

    dijkstra(graph, n, start - 1);

    return 0;
}