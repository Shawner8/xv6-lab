#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  // parse args.
  uint64 base, mask;
  int len;
  argaddr(0, &base);
  argint(1, &len);
  argaddr(2, &mask);

  // limit the number of pages that can be scanned to be the bit num of mask.
  unsigned int abits = 0;
  if (len > sizeof(abits)*8)
  {
    printf("The scanned number of pages is too large!\n");
    return -1;
  }
  
  // scan the pagetable.
  uint64 va0 = PGROUNDDOWN(base);
  pagetable_t pagetable = myproc()->pagetable;
  // vmprint(pagetable);
  for (int i = 0; i < len; ++i)
  {
    if(va0 >= MAXVA)
      return -1;
    pte_t *pte = walk(pagetable, va0, 0);
    if(pte == 0 || (*pte & PTE_V) == 0 || (*pte & PTE_U) == 0 ||
       (*pte & PTE_W) == 0)
      return -1;
    if ((*pte & PTE_A) != 0)
    {
      abits |= (1 << i);
      *pte &= ~PTE_A;
    }
    va0 += PGSIZE;
  }
  // vmprint(pagetable);
  
  return copyout(pagetable, mask, (char *)&abits, sizeof(abits));
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
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
