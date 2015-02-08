#include "types.h"
#include "stat.h"
#include "user.h"
#include "traps.h"
char *ptr;
void handler1()
{
 //__asm__("movl $4,4(%ebp);"); 
 ptr -= 4;
 int *eip = (int*)ptr;
 *eip += 4;
 //printf(1,"in.. %d",*eip);
 printf(1,"\nDivide by Zero Error... \n");
}

void handler2()
{
 printf(1,"\nSegmentation fault..Exiting from program..\n");
 exit();
}

int
main(int argc, char **argv)
{
 __asm__("movl %esp,%eax");
 __asm__("movl %%eax,%0" : "=r"(ptr));
  
  if(argc < 2){
    printf(1,"Program takes 2 arg...Enter 2 integers for division..\n");
    exit();
  }
  int x= -1;
  x=signal(SIGFPE,handler1);
  printf(1,"\nSignal system call return value for SIGFPE: %d",x);
  x=signal(SIGSEGV,handler2);
  printf(1,"\nSignal system call return value for SIGEGV: %d",x);
  /*Potential of generating divide by zero error*/
  int num = 0;
  num = atoi(argv[1])/atoi(argv[2]);
  printf(1,"\nDivision result: %d\n",num);
  /*Creating Segmentation fault*/
  int *ptr =(int*)112233;
  *ptr=100;
  printf(1,"%d",*ptr);
  exit();
}
