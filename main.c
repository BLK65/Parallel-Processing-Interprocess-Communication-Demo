#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define DATA_FILE "data.bin"
#define NUM_CHILDREN 7

int main() {
    int pipe_fd[2];
    pid_t child_pids[NUM_CHILDREN];
    int total_sum = 0;

    // Create a pipe
    if (pipe(pipe_fd) == -1)
    {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Create P0 and P1
    for (int i = 0; i < 2; ++i)
    {
        pid_t child_pid = fork();

        if (child_pid == -1)
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }

        // Child process
        if (child_pid == 0)
        {
            int file = open(DATA_FILE, O_RDONLY);

            if (file == -1)
            {
                perror("File opening error");
                exit(EXIT_FAILURE);
            }

            int value;
            lseek(file, i * sizeof(int), SEEK_SET); // Move to the relevant position
            read(file, &value, sizeof(int)); // Read the integer value
            value *= value; // Square the value

            // Write to the pipe
            if (write(pipe_fd[1], &value, sizeof(int)) == -1)
            {
                perror("Error writing to the pipe");
                exit(EXIT_FAILURE);
            }

            close(file);
            close(pipe_fd[1]); // Close the writing end of the pipe
            exit(EXIT_SUCCESS);
        }
        else
        {
            child_pids[i] = child_pid;
        }
    }

    // Wait for the termination of child processes with P0 and P1 PIDs
    for (int i = 0; i < 2; ++i)
    {
        waitpid(child_pids[i], NULL, 0);
    }

    // Create P2 and P3
    for (int i = 2; i < 4; ++i)
    {
        pid_t child_pid = fork();

        if (child_pid == -1)
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }

        // Child process
        if (child_pid == 0)
        {
            int file = open(DATA_FILE, O_RDONLY);

            if (file == -1)
            {
                perror("File opening error");
                exit(EXIT_FAILURE);
            }

            int value;
            lseek(file, i * sizeof(int), SEEK_SET); // Move to the relevant position
            read(file, &value, sizeof(int)); // Read the integer value
            value *= value; // Square the value

            // Write to the pipe
            if (write(pipe_fd[1], &value, sizeof(int)) == -1)
            {
                perror("Error writing to the pipe");
                exit(EXIT_FAILURE);
            }

            close(file);
            close(pipe_fd[1]); // Close the writing end of the pipe
            exit(EXIT_SUCCESS);
        }
        else
        {
            child_pids[i] = child_pid;
        }
    }

    // Wait for the termination of child processes with P2 and P3 PIDs
    for (int i = 2; i < 4; ++i)
    {
        waitpid(child_pids[i], NULL, 0);
    }

    // Create P4, P5, P6, P7
    for (int i = 4; i < NUM_CHILDREN; ++i)
    {
        pid_t child_pid = fork();

        if (child_pid == -1)
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }

        // Child process
        if (child_pid == 0)
        {
            int file = open(DATA_FILE, O_RDONLY);

            if (file == -1)
            {
                perror("File opening error");
                exit(EXIT_FAILURE);
            }

            int value;
            lseek(file, i * sizeof(int), SEEK_SET); // Move to the relevant position
            read(file, &value, sizeof(int)); // Read the integer value
            value *= value; // Square the value

            // Write to the pipe
            if (write(pipe_fd[1], &value, sizeof(int)) == -1)
            {
                perror("Error writing to the pipe");
                exit(EXIT_FAILURE);
            }

            close(file);
            close(pipe_fd[1]); // Close the writing end of the pipe
            exit(EXIT_SUCCESS);
        }
        else
        {
            child_pids[i] = child_pid;
        }
    }

    // Wait for the termination of child processes with P4, P5, P6, P7 PIDs
    for (int i = 4; i < NUM_CHILDREN; ++i)
    {
        waitpid(child_pids[i], NULL, 0);
    }

    // Parent process
    close(pipe_fd[1]); // Close the writing end of the pipe

    // Read from the pipe and accumulate the values
    int child_value;
    while (read(pipe_fd[0], &child_value, sizeof(int)) > 0)
    {
        total_sum += child_value;
    }

    close(pipe_fd[0]); // Close the reading end of the pipe

    printf("%d\n", total_sum);

    return 0;
}
