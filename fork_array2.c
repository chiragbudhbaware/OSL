/* reverser.c
 * Program that prints its command-line integer arguments in reverse order.
 * Compile to executable named "reverser" and execve'd by parent/child.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc <= 1){
        printf("No numbers received.\n");
        return 0;
    }

    printf("Reverser: numbers in reverse order: ");
    /* argv[1]..argv[argc-1] are the numbers passed by parent */
    for(int i = argc - 1; i >= 1; --i){
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}
