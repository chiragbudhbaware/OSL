/* scan.c
 * Simple SCAN (Elevator) Disk Scheduling Algorithm
 * Easy to understand version.
 *
 * Compile:
 *   gcc scan.c -o scan
 * Run:
 *   ./scan
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
    int n, head, disk_size, direction;
    printf("Enter number of requests: ");
    scanf("%d", &n);

    int req[n];
    printf("Enter the request queue:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter total disk size (e.g., 200): ");
    scanf("%d", &disk_size);

    printf("Enter head direction (1 for high end, 0 for low end): ");
    scanf("%d", &direction);

    // sort the request queue
    sort(req, n);

    int total_movement = 0;
    int index = 0;

    // find the first request greater than head
    for (int i = 0; i < n; i++) {
        if (req[i] > head) {
            index = i;
            break;
        }
    }

    printf("\nSequence of head movement:\n");

    // if direction is towards high end
    if (direction == 1) {
        // move from current head towards end
        for (int i = index; i < n; i++) {
            printf("Move from %d to %d\n", head, req[i]);
            total_movement += abs(req[i] - head);
            head = req[i];
        }

        // at the end of the disk
        printf("Move from %d to %d (end of disk)\n", head, disk_size - 1);
        total_movement += abs((disk_size - 1) - head);
        head = disk_size - 1;

        // now move back toward beginning
        for (int i = index - 1; i >= 0; i--) {
            printf("Move from %d to %d\n", head, req[i]);
            total_movement += abs(req[i] - head);
            head = req[i];
        }
    } else { // direction towards low end
        // move toward beginning first
        for (int i = index - 1; i >= 0; i--) {
            printf("Move from %d to %d\n", head, req[i]);
            total_movement += abs(req[i] - head);
            head = req[i];
        }

        // reach the beginning of disk
        printf("Move from %d to 0 (start of disk)\n", head);
        total_movement += head;
        head = 0;

        // now move toward high end
        for (int i = index; i < n; i++) {
            printf("Move from %d to %d\n", head, req[i]);
            total_movement += abs(req[i] - head);
            head = req[i];
        }
    }

    printf("\nTotal head movement = %d\n", total_movement);
    printf("Average head movement = %.2f\n", (float) total_movement / n);

    return 0;
}
