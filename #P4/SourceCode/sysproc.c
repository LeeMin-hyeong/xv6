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
sys_ssualloc(void)
{
  uint addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  // 잘못된 인자 처리
  if(n <= 0 || n % PGSIZE)
    return -1;

  // 할당 전 가상 메모리 크기를 주소로 리턴
  addr = myproc()->sz;
  // 가상 메모리 크기 인자만큼 증가
  myproc()->sz += n;

  return addr;
}

int
sys_getvp(void)
{
  // 가상 메모리 크기 / PGSIZE
  return myproc()->sz / PGSIZE;
}

int
sys_getpp(void)
{
  pte_t *pde;
  pte_t *pgtab;
  int psz;
  uint va;

  psz = 0;
  // 가상 주소 0 부터 PGSIZE 단위로 접근
  for (va = 0; va < myproc()->sz; va += PGSIZE) {
    // 페이지 디렉토리 엔트리
    pde = &myproc()->pgdir[PDX(va)];
    if(*pde & PTE_P){
      // 페이지 테이블
      pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
      // 페이지 테이블 엔트리
      pte_t *pte = &pgtab[PTX(va)];
      if (pte != 0 && *pte & PTE_P) {
        psz += PGSIZE;
      }
    }
  }
  // 물리 메모리 크기 / PGSIZE
  return psz / PGSIZE;
}