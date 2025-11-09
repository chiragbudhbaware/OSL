/* sjf_preemptive.c
 * Shortest Job First (Preemptive) / Shortest Remaining Time First (SRTF)
 * - Input: number of processes, arrival times and burst times
 * - Simulates time unit by unit and always runs the process with shortest remaining time
 * - Outputs completion time, turnaround time, waiting time, and averages
 */

#include <stdio.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    int arrival[n], burst[n], remaining[n], completed[n];
    int completion[n];
    for (int i = 0; i < n; ++i) {
        printf("Process %d arrival time: ", i+1);
        scanf("%d", &arrival[i]);
        printf("Process %d burst time: ", i+1);
        scanf("%d", &burst[i]);
        remaining[i] = burst[i];   // remaining time initially equals burst time
        completed[i] = 0;          // not completed yet
        completion[i] = 0;
    }

    int completed_count = 0;
    int time = 0;
    int prev_time = -1;
    double total_wait = 0, total_turnaround = 0;

    /* Run simulation until all processes complete */
    while (completed_count < n) {
        int idx = -1;
        int min_rem = INT_MAX;

        /* pick process with smallest remaining time among arrived and not completed */
        for (int i = 0; i < n; ++i) {
            if (!completed[i] && arrival[i] <= time && remaining[i] < min_rem && remaining[i] > 0) {
                min_rem = remaining[i];
                idx = i;
            }
        }

        if (idx == -1) {
            // no process ready at this time -> idle CPU, advance time
            time++;
            continue;
        }

        // execute selected process for 1 time unit
        remaining[idx]--;
        time++;

        // if process finished now
        if (remaining[idx] == 0) {
            completed[idx] = 1;
            completed_count++;
            completion[idx] = time;                       // completion time
            int turnaround = completion[idx] - arrival[idx];
            int waiting = turnaround - burst[idx];
            total_turnaround += turnaround;
            total_wait += waiting;
        }
    }

    // Print results
    printf("\nP#\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; ++i) {
        int tat = completion[i] - arrival[i];
        int wt = tat - burst[i];
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", i+1, arrival[i], burst[i], completion[i], tat, wt);
    }

    printf("\nAverage Turnaround Time = %.2f\n", total_turnaround / n);
    printf("Average Waiting Time    = %.2f\n", total_wait / n);

    return 0;
}
