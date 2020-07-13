#include "kernel/types.h"
#include "user/user.h"
#define BUFFER_SIZE 256
int main(int argc, const char* argv[]){
    int parent_fd[2];
    int child_fd[2];
    if (pipe(parent_fd) < 0){
        fprintf(2, "parent_fd pipe error");
        exit();
    }

    if (pipe(child_fd) < 0){
        fprintf(2, "parent_fd pipe error");
	exit();
    }
    int pid;
    char buffer[BUFFER_SIZE];
    pid = fork();
    if (pid < 0){
        fprintf(2, "fork failed");
        exit();
    }
    if (pid == 0){
       close(parent_fd[1]);
       close(child_fd[0]);
       read(parent_fd[0], buffer, BUFFER_SIZE);
       fprintf(1,"%d:received %s\n", getpid(), buffer);
       write(child_fd[1], "pong", strlen("pong"));
    } else if (pid > 0){
       close(parent_fd[0]);
       close(child_fd[1]);
       write(parent_fd[1], "ping", strlen("ping"));
       read(child_fd[0], buffer, BUFFER_SIZE);
       fprintf(1,"%d:received %s\n", getpid(), buffer);
    }
    exit();
}
