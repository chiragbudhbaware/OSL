#include <stdio.h>

int main() {
    int n, time_quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int arrival[n], burst[n], remaining[n];
    int completion[n], waiting[n], turnaround[n];
    int done[n];

    for (int i = 0; i < n; i++) {
        printf("Enter arrival time of P%d: ", i+1);
        scanf("%d", &arrival[i]);
        printf("Enter burst time of P%d: ", i+1);
        scanf("%d", &burst[i]);
        remaining[i] = burst[i];
        done[i] = 0;
    }

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    int time = 0, completed = 0;

    while (completed < n) {
        int executed = 0;

        for (int i = 0; i < n; i++) {
            if (arrival[i] <= time && remaining[i] > 0) {
                executed = 1; // found a ready process

                if (remaining[i] > time_quantum) {
                    time += time_quantum;
                    remaining[i] -= time_quantum;
                } else {
                    time += remaining[i];
                    remaining[i] = 0;
                    done[i] = 1;
                    completed++;

                    completion[i] = time;
                    turnaround[i] = completion[i] - arrival[i];
                    waiting[i] = turnaround[i] - burst[i];
                }
            }
        }

        // If no process arrived yet, just increment time
        if (!executed)
            time++;
    }

    // Print results
    printf("\nP#\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               i+1, arrival[i], burst[i],
               completion[i], turnaround[i], waiting[i]);
    }

    // Calculate averages
    float avg_tat = 0, avg_wt = 0;
    for (int i = 0; i < n; i++) {
        avg_tat += turnaround[i];
        avg_wt += waiting[i];
    }
    avg_tat /= n;
    avg_wt /= n;

    printf("\nAverage Turnaround Time = %.2f", avg_tat);
    printf("\nAverage Waiting Time    = %.2f\n", avg_wt);

    return 0;
}
