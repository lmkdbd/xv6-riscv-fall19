#include "kernel/types.h"
#include "user/user.h"

#define ARRAY_SIZE 128
int generate_array(int begin, int end, int step, int *array, int size){
    int count = 0;
    for (int i = begin; i <= end; i = i + step){
        array[count] = i;
        count = count + 1;
    }
    return count;
}

void create(int prime, int parent_fd){
    int fd[2];
    if (pipe(fd) < 0){
        fprintf(2, "parent_fd pipe error\n");
        exit();
    }
    int pid;
    pid = fork();
    if (pid > 0){
        //parent
        fprintf(1, "prime:%d\n", prime);
        close(fd[0]);
        int number = 0;
        int count = read(parent_fd, &number, sizeof(number));
        while (count) {
            if (number % prime){
                write(fd[1], &number, sizeof(number));
            }
            count = read(parent_fd, &number, sizeof(number));
        }
		printf("pid:%d exit!\n",pid);
        close(fd[1]);
    } else {
        //children
        close(fd[1]);
        int number = 0;
        int count = read(fd[0],&number, sizeof(number));
        if (count){
			create(number, fd[0]);
		}
        close(fd[0]);
    }
}

int main(int argc, const char* argv[]){
    if (argc != 2){
        fprintf(2, "usage: primes number\n");
        exit();
    }
    int end_number = atoi(argv[1]);
    int array[ARRAY_SIZE];
    int prime = 2;
    int count = generate_array(prime, end_number, 1, array, ARRAY_SIZE);
	printf("the count is:%d\n",count);
    int fd[2];
    if (pipe(fd) < 0){
        fprintf(2, "fd pipe error\n");
        exit();
    }

    int pid;
    pid = fork();
    if (pid > 0){
        //parent
        close(fd[0]);
        fprintf(1, "prime:%d\n", prime);
        for (int i = 0; i < count; i++){
             if (array[i] % prime){
                write(fd[1], &(array[i]), sizeof(int));
             }
        }
        close(fd[1]);
    } else {
        //children 
        close(fd[1]);
        int number = 0;
        int count = read(fd[0],&number, sizeof(number));
		if (count){
        	create(number, fd[0]);
		}
        close(fd[0]);
    }
	printf("main exit!\n");
    exit();
}
