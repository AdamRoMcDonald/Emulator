#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define THREAD_COUNT 3
#define ITERATIONS 100000

pthread_t threads[THREAD_COUNT];

void* thread_function(void* arg) {
    long thread_id = (long)arg;
    pid_t pid = getpid();

    for (int i = 0; i < ITERATIONS; i++) {
        printf("I am thread %d.%ld\n", pid, thread_id);
    }

    return NULL;
}

void create_threads_in_reverse() {
    for (long i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void*)(THREAD_COUNT - i));
    }

    // Reverse order of joining threads
    for (long i = THREAD_COUNT - 1; i >= 0; i--) {
        pthread_join(threads[i], NULL);
    }
}

void fork_processes_in_reverse() {
    pid_t pid = fork();

    if (pid == 0) {
        // Fork P3
        pid_t pid2 = fork();
        if (pid2 == 0) {
            // P3
            printf("Forked Process 3: PID = %d\n", getpid());
            create_threads_in_reverse();
            exit(0);
        } else {
            waitpid(pid2, NULL, 0); // Wait for P3
        }

        // P2
        printf("Forked Process 2: PID = %d\n", getpid());
        create_threads_in_reverse();
        exit(0);
    } else {
        waitpid(pid, NULL, 0); // Wait for P2

        // P1
        printf("Forked Process 1: PID = %d\n", getpid());
        create_threads_in_reverse();
    }
}

int main() {
    fork_processes_in_reverse();
    return 0;
}
