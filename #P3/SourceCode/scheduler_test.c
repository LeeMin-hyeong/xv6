#include "types.h"
#include "stat.h"
#include "user.h"
#define PNUM 3

void scheduler_func(void)
{
    int pid = getpid();
    for(int i=0; i<PNUM; i++){
        if(getpid() == pid)
            fork();
    }
    if(getpid() == pid+1)
        set_sche_info(1, 110);
    else if(getpid() == pid+2)
        set_sche_info(10, 60);
    else 
        set_sche_info(11, 60);
    
    if(getpid() != pid)
        while(1);
    else
        for(int i=0; i<PNUM; i++)
            wait();
}

int main(void)
{
    scheduler_func();
    exit();
}