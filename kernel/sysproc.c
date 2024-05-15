#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//trace系统调用，接收一个int参数，赋值到进程结构体中的mask
uint64
sys_trace(void)
{
  int n;
  argint(0,&n);//接收参数
  myproc()->mask = n;//赋给mask
  return 0;
}

extern uint64 get_free_memory();
extern uint64 get_used_proc();

uint64 
sys_sysinfo(){
  uint64 st;
  argaddr(0, &st);//获取从用户空间传入的指针。
  struct sysinfo p;//将信息存在结构体中
  p.nproc = get_used_proc();
  p.freemem = get_free_memory();
  if(copyout(myproc()->pagetable, st, (char *)&p, sizeof(p)) < 0)//拷贝回用户空间
    return -1;
  return 0;
}
