#include "kernel/types.h"
#include "user/user.h"

int main(int argc, const char* argv[]){
    if (argc != 2){
        fprintf(2,"usage: sleep seconds\n");
        exit();
    }
    int second = atoi(argv[1]);
    sleep(second);
    exit();
}
