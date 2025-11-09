/* parent.c
 * Read integers, sort them, fork. Child execve's "reverser" passing sorted numbers
 * as command-line args. Parent waits for child to finish.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ; /* needed for execve */

void selection_sort(int a[], int n){
    for(int i=0;i<n-1;i++){
        int min=i;
        for(int j=i+1;j<n;j++) if(a[j]<a[min]) min=j;
        if(min!=i){ int t=a[i]; a[i]=a[min]; a[min]=t; }
    }
}

int main(){
    int n;
    printf("Enter number of integers: ");
    if(scanf("%d", &n)!=1 || n<=0){
        fprintf(stderr,"Invalid input\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    if(!arr){ perror("malloc"); return 1; }

    printf("Enter %d integers:\n", n);
    for(int i=0;i<n;i++){
        if(scanf("%d", &arr[i])!=1){ fprintf(stderr,"Invalid number\n"); free(arr); return 1;}
    }

    /* Sort the array in parent before passing to child */
    selection_sort(arr, n);

    pid_t pid = fork();
    if(pid < 0){
        perror("fork");
        free(arr);
        return 1;
    }

    if(pid == 0){
        /* Child: build argv for execve and exec the reverser program */
        /* argv layout: argv[0] = "./reverser", argv[1..n] = sorted numbers as strings, argv[n+1] = NULL */
        char **args = malloc((n + 2) * sizeof(char *));
        if(!args){ perror("malloc"); _exit(1); }

        args[0] = "./reverser"; /* path/program name */

        for(int i=0;i<n;i++){
            /* allocate small buffer for each number string */
            args[i+1] = malloc(16); /* enough for integer string */
            if(!args[i+1]){ perror("malloc"); /* free previously allocated */ 
                for(int k=1;k<=i;k++) free(args[k]);
                free(args);
                _exit(1);
            }
            sprintf(args[i+1], "%d", arr[i]);
        }
        args[n+1] = NULL;

        /* execve requires a full path or relative like "./reverser". Pass environ */
        execve("./reverser", args, environ);

        /* If execve returns, it's an error */
        perror("execve");
        for(int i=1;i<=n;i++) free(args[i]);
        free(args);
        _exit(1);
    } else {
        /* Parent: wait for child to finish */
        wait(NULL);
        printf("Parent: sorted array was: ");
        for(int i=0;i<n;i++) printf("%d ", arr[i]);
        printf("\n");
        free(arr);
    }

    return 0;
}
