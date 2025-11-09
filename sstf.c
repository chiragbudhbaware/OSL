/* sstf.c
 * Shortest Seek Time First (SSTF) disk scheduling
 * Simple, easy-to-read implementation (O(n^2))
 *
 * Compile:
 *   gcc sstf.c -o sstf
 * Run:
 *   ./sstf
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter number of disk I/O requests: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of requests.\n");
        return 1;
    }

    int *req = malloc(n * sizeof(int));
    int *done = malloc(n * sizeof(int));
    if (!req || !done) {
        perror("malloc");
        return 1;
    }

    printf("Enter the request queue (space separated):\n");
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &req[i]) != 1) {
            printf("Invalid input.\n");
            free(req); free(done);
            return 1;
        }
        done[i] = 0; // not serviced yet
    }

    int head;
    printf("Enter initial head position: ");
    if (scanf("%d", &head) != 1) {
        printf("Invalid head position.\n");
        free(req); free(done);
        return 1;
    }

    int serviced = 0;
    int current = head;
    long total_movement = 0;

    printf("\nService order and movements:\n");
    printf("Start at head = %d\n", head);

    while (serviced < n) {
        int best_idx = -1;
        int best_dist = INT_MAX;

        // find nearest unserviced request
        for (int i = 0; i < n; ++i) {
            if (!done[i]) {
                int dist = abs(req[i] - current);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_idx = i;
                }
            }
        }

        // service the best_idx
        if (best_idx == -1) break; // safety
        printf("Move from %d to %d  (seek = %d)\n", current, req[best_idx], best_dist);
        total_movement += best_dist;
        current = req[best_idx];
        done[best_idx] = 1;
        serviced++;
    }

    printf("\nTotal head movement = %ld\n", total_movement);
    printf("Average seek per request = %.2f\n", (double)total_movement / n);

    free(req);
    free(done);
    return 0;
}
