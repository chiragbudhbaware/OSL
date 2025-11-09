/* c_look.c
 * Simple C-LOOK Disk Scheduling Algorithm
 * Easy to understand and explain.
 *
 * Compile:
 *   gcc c_look.c -o c_look
 * Run:
 *   ./c_look
 */

#include <stdio.h>
#include <stdlib.h>

void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

int main() {
    int n, head;
    printf("Enter number of requests: ");
    scanf("%d", &n);

    int req[n];
    printf("Enter the request queue:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    sort(req, n);  // sort the request queue

    int total_movement = 0;
    int index = 0;

    // find the first request greater than the head position
    for (int i = 0; i < n; i++) {
        if (req[i] > head) {
            index = i;
            break;
        }
    }

    printf("\nSequence of head movement:\n");

    // Move from head to higher requests first
    for (int i = index; i < n; i++) {
        printf("Move from %d to %d\n", head, req[i]);
        total_movement += abs(req[i] - head);
        head = req[i];
    }

    // Jump to the lowest request (C-LOOK circular move)
    if (index > 0) {
        printf("Jump from %d to %d\n", head, req[0]);
        total_movement += abs(head - req[0]);
        head = req[0];

        // Service remaining lower requests
        for (int i = 0; i < index; i++) {
            printf("Move from %d to %d\n", head, req[i]);
            total_movement += abs(req[i] - head);
            head = req[i];
        }
    }

    printf("\nTotal head movement = %d\n", total_movement);
    printf("Average head movement = %.2f\n", (float) total_movement / n);

    return 0;
}
