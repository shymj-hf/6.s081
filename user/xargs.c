    #include "kernel/types.h"
    #include "user/user.h"
    #include "kernel/param.h"
    int main(int argc, char *argv[]){
        char *p[MAXARG];
        int i;
        for(i=1; i < argc; i++)
            p[i-1] = argv[i];//把参数都放进p
        p[argc-1]  = malloc(512);//在最后一个参数后面分配空间
        p[argc] = 0;//在空间后面+0
        while(gets(p[argc-1],512)){ //gets函数一次读取一行
            if(p[argc-1][0] == 0)break; //已读完
            if(p[argc-1][strlen(p[argc-1])-1]=='\n'){
                p[argc-1][strlen(p[argc-1])-1] = 0;
            } //该函数会将末尾换行保留，故需去掉换行符。
        	if(fork()==0) 
                exec(argv[1],p);
    	    else
    	        wait(0);
        }
        exit(0);
    }