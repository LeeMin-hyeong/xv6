#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_date(void)
{
  // argument를 담을 구조체 포인터 변수 선언
  struct rtcdate *d;
  // 0번 인자, 인자 포인터, 인자 포인터의 크기
  argptr(0, (void *)&d, sizeof(struct rtcdate));
  // cmostime을 통해 현재 시간을 인자에 저장
  cmostime(d);
  return 0;
}

int
sys_alarm(void)
{
  // argument를 담을 정수 변수 선언
  int n;
  // 인자 받아오기
  if(argint(0, &n) < 0)
    return -1;
  // 현재 프로세스의 알람 타이머를 인자의 값으로 설정
  // 에뮬레이터 성능으로 인해 60 ticks/sec
  // myproc()->alarm_timer = n*60;
  // xv6의 경우 100ticks/sec
  myproc()->alarm_timer = n*100;
  return 0;
}
