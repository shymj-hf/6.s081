#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

__attribute__((noreturn)) 
void primes(int p[2]){
    close(p[1]);
    int first;
    int data;
    if(read(p[0], &first, sizeof(int)) == sizeof(int)){
        fprintf(1,"prime %d\n",first);
        int q[2];
        pipe(q);
        while(read(p[0], &data, sizeof(int)) == sizeof(int)){
            if(data%first){
                write(q[1], &data, sizeof(int));
            }
        }
        close(p[0]);
        close(q[1]);
        if(fork() == 0){
            primes(q);
        }else{
            close(q[0]);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc,char* argv[]){
    int p[2];
    pipe(p);
    for(int i = 2; i <= 35; i++){
        write(p[1], &i, sizeof(int));
    }
    if(fork() == 0){
        primes(p);
    }else{
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}