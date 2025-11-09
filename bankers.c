#include <stdio.h>
#include <stdlib.h>

void calculate_need(int **need, int **max, int **alloc, int n, int m) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            need[i][j] = max[i][j] - alloc[i][j];
}

int is_safe(int **alloc, int **max, int *available, int n, int m, int *safe_seq) {
    int *work = malloc(m * sizeof(int));
    int *finish = malloc(n * sizeof(int));
    int **need = malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) need[i] = malloc(m * sizeof(int));

    for (int j = 0; j < m; ++j) work[j] = available[j];
    for (int i = 0; i < n; ++i) finish[i] = 0;

    calculate_need(need, max, alloc, n, m);

    int count = 0;
    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; ++i) {
            if (!finish[i]) {
                int can_allocate = 1;
                for (int j = 0; j < m; ++j) {
                    if (need[i][j] > work[j]) { can_allocate = 0; break; }
                }
                if (can_allocate) {
                    // Assume process i runs to completion
                    for (int j = 0; j < m; ++j) work[j] += alloc[i][j];
                    safe_seq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            // No process could be satisfied in this pass -> unsafe
            for (int i = 0; i < n; ++i) free(need[i]);
            free(need);
            free(work);
            free(finish);
            return 0;
        }
    }

    for (int i = 0; i < n; ++i) free(need[i]);
    free(need);
    free(work);
    free(finish);
    return 1;
}

int main() {
    int n, m;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input\n"); return 1; }
    printf("Enter number of resource types: ");
    if (scanf("%d", &m) != 1 || m <= 0) { printf("Invalid input\n"); return 1; }

    // allocate matrices/vectors
    int **alloc = malloc(n * sizeof(int*));
    int **max = malloc(n * sizeof(int*));
    int *available = malloc(m * sizeof(int));
    for (int i = 0; i < n; ++i) {
        alloc[i] = malloc(m * sizeof(int));
        max[i]   = malloc(m * sizeof(int));
    }

    printf("Enter Allocation matrix (each row for a process, %d numbers per row):\n", m);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            scanf("%d", &alloc[i][j]);

    printf("Enter Max matrix (each row for a process, %d numbers per row):\n", m);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            scanf("%d", &max[i][j]);

    printf("Enter Available resources vector (%d numbers):\n", m);
    for (int j = 0; j < m; ++j)
        scanf("%d", &available[j]);

    int *safe_seq = malloc(n * sizeof(int));
    int safe = is_safe(alloc, max, available, n, m, safe_seq);

    if (safe) {
        printf("\nSystem is in SAFE state.\nSafe sequence is: ");
        for (int i = 0; i < n; ++i) {
            printf("P%d", safe_seq[i]);
            if (i < n-1) printf(" -> ");
        }
        printf("\n");
    } else {
        printf("\nSystem is in UNSAFE state (no safe sequence).\n");
    }

    // cleanup
    for (int i = 0; i < n; ++i) {
        free(alloc[i]);
        free(max[i]);
    }
    free(alloc);
    free(max);
    free(available);
    free(safe_seq);

    return 0;
}
