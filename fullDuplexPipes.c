#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define MARKER "<END_OF_FILE>\n"

int count_chars(const char *s) {
    return strlen(s);
}

int count_words(const char *s) {
    int in_word = 0, words = 0;
    for (const char *p = s; *p; ++p) {
        if (*p==' ' || *p=='\t' || *p=='\n' || *p=='\r') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }
    return words;
}

int count_lines(const char *s) {
    /* For this line-oriented example, treat each input as a single line.
       But also count '\n' in case the input contains multiple lines. */
    int lines = 0;
    for (const char *p = s; *p; ++p) if (*p == '\n') lines++;
    return (lines == 0) ? 1 : lines;
}

int main(void) {
    int pipe1[2]; // parent -> child
    int pipe2[2]; // child  -> parent

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /********** Child process **********/
        // Close unused ends
        close(pipe1[1]); // child will read from pipe1[0]
        close(pipe2[0]); // child will write to pipe2[1]

        FILE *in = fdopen(pipe1[0], "r");
        FILE *out = fdopen(pipe2[1], "w");
        if (!in || !out) { perror("fdopen child"); _exit(1); }

        char buf[BUF_SIZE];
        while (fgets(buf, sizeof(buf), in) != NULL) {
            // remove trailing newline for processing
            size_t len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';

            // Check for exit command
            if (strcmp(buf, "exit") == 0) {
                // Optionally send a final acknowledgement and marker
                fprintf(out, "Child: received exit. Terminating.\n");
                fprintf(out, MARKER);
                fflush(out);
                break;
            }

            // Count chars, words, lines
            int chars = count_chars(buf);
            int words = count_words(buf);
            int lines = count_lines(buf);

            // Append results to file "output.txt"
            FILE *f = fopen("output.txt", "a");
            if (!f) { perror("fopen in child"); /* continue but still send file contents*/ }
            else {
                fprintf(f, "Input : %s\n", buf);
                fprintf(f, "Chars : %d, Words : %d, Lines : %d\n\n", chars, words, lines);
                fclose(f);
            }

            // Send entire file contents back to parent, line by line
            f = fopen("output.txt", "r");
            if (!f) {
                // If file missing or cannot be opened, send an error message instead
                fprintf(out, "Child: error opening output.txt for reading.\n");
                fprintf(out, MARKER);
                fflush(out);
            } else {
                char line[BUF_SIZE];
                while (fgets(line, sizeof(line), f) != NULL) {
                    fputs(line, out);
                }
                fclose(f);
                // send marker to indicate end of file content
                fputs(MARKER, out);
                fflush(out);
            }
        }

        // cleanup
        fclose(in);
        fclose(out);
        _exit(0);

    } else {
        /********** Parent process **********/
        // Close unused ends
        close(pipe1[0]); // parent will write to pipe1[1]
        close(pipe2[1]); // parent will read from pipe2[0]

        FILE *to_child = fdopen(pipe1[1], "w");
        FILE *from_child = fdopen(pipe2[0], "r");
        if (!to_child || !from_child) { perror("fdopen parent"); return 1; }

        char input[BUF_SIZE];

        while (1) {
            printf("Enter sentence (type 'exit' to quit): ");
            if (fgets(input, sizeof(input), stdin) == NULL) break;

            // remove trailing newline
            size_t ln = strlen(input);
            if (ln > 0 && input[ln-1] == '\n') input[ln-1] = '\0';

            // send to child
            fprintf(to_child, "%s\n", input);
            fflush(to_child);

            // read response from child until MARKER line is found
            printf("---- Received from child (file contents) ----\n");
            char line[BUF_SIZE];
            while (fgets(line, sizeof(line), from_child) != NULL) {
                if (strcmp(line, MARKER) == 0) break;
                // print each received line
                fputs(line, stdout);
            }
            printf("---- End of child's output ----\n\n");

            if (strcmp(input, "exit") == 0) break;
        }

        // wait for child to exit
        fclose(to_child);
        fclose(from_child);
        wait(NULL);
        printf("Parent: child exited, program terminating.\n");
    }

    return 0;
}
