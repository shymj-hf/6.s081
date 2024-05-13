#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[]){
    int p[2];
    int q[2];
    pipe(p);
    pipe(q);
    if (fork() == 0) {
        char data;
        read(p[0],&data,1);
        fprintf(1,"%d: received ping\n", getpid());
        close(p[0]);
        close(p[1]);
        write(q[1], &data, 1);
        close(q[0]);
        close(q[1]);
        exit(0);
    } else {
        char data;
        write(p[1], "1", 1);
        close(p[0]);
        close(p[1]);
        wait(0);
        read(q[0],&data,1);
        fprintf(1,"%d: received pong\n", getpid());
        close(q[0]);
        close(q[1]);
        exit(0);
    }
}