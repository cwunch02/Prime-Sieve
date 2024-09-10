#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

void sieve(int leftfd) {
    int prime;
    if (read(leftfd, &prime, sizeof(int)) <= 0) {
        close(leftfd);
        exit(0);
    }

    printf("pid=%d prime %d\n", getpid(), prime);

    int rightfd;
    int childPid = 0;

    while (true) {
        int n;
        if (read(leftfd, &n, sizeof(int)) <= 0) {
            break;
        }

        if (n % prime != 0) {
            if (rightfd == 0) {
                int pipefds[2];
                if (pipe(pipefds) == -1) {
                    perror("pipe");
                    exit(1);
                }

                rightfd = pipefds[1];
                childPid = fork();

                if (childPid == -1) {
                    perror("fork");
                    exit(1);
                }

                if (childPid == 0) {
                    close(pipefds[1]);
                    close(leftfd);
                    sieve(pipefds[0]);
                } else {
                    close(pipefds[0]);
                }
            }

            if (write(rightfd, &n, sizeof(int)) == -1) {
                perror("write");
                exit(1);
}
        }
    }

    if (childPid != 0) {
        close(rightfd);
        wait(NULL);
    }

    close(leftfd);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s N\n", argv[0]);
        return 1;
    }
 int N = atoi(argv[1]);

    if (N < 2) {
        fprintf(stderr, "N must be greater than or equal to 2.\n");
        return 1;
    }

    int pipefds[2];
    pipe(pipefds);

    pid_t childPid = fork();

    if (childPid == -1) {
        perror("fork");
        return 1;
    }

if (childPid == 0) {
        close(pipefds[1]);
        sieve(pipefds[0]);
    } else {
        close(pipefds[0]);
        for (int i = 2; i <= N; i++) {
            if (write(pipefds[1], &i, sizeof(int)) == -1) {
                perror("write");
                return EXIT_FAILURE;
            }
        }
        close(pipefds[1]);
        wait(NULL);
    }

    return 0;
}
