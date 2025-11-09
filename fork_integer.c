#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Simple bubble sort for child */
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-1-i; ++j) {
            if (arr[j] > arr[j+1]) {
                int t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
            }
        }
    }
}

/* Simple selection sort for parent */
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n-1; ++i) {
        int min_idx = i;
        for (int j = i+1; j < n; ++j)
            if (arr[j] < arr[min_idx]) min_idx = j;
        if (min_idx != i) {
            int t = arr[i]; arr[i] = arr[min_idx]; arr[min_idx] = t;
        }
    }
}

void print_array(const char *who, int arr[], int n) {
    printf("%s sorted array: ", who);
    for (int i = 0; i < n; ++i) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n;

    printf("Enter number of integers: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid number.\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &arr[i]) != 1) {
            fprintf(stderr, "Invalid input.\n");
            free(arr);
            return 1;
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(arr);
        return 1;
    }

    if (pid == 0) {
        /* Child process: sort using bubble sort and print */
        bubble_sort(arr, n);
        print_array("Child", arr, n);
        free(arr);
        _exit(0); /* exit child without flushing parent's buffers */
    } else {
        /* Parent process: wait for child to finish first */
        wait(NULL); /* parent waits for child */
        /* Then parent sorts using selection sort and prints */
        selection_sort(arr, n);
        print_array("Parent", arr, n);
        free(arr);
    }

    return 0;
}