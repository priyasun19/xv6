Name: Priya Sundaresan
Part 1: Done
Part 2: Done
Changes - File names: Usys.S, syscall.h, syscall.c, sysproc.c, trap.c, traps.h, proc.h, proc.c, types.h, user.h, foo.c

Explanation: 
xv6 does not support user-defined signal handler. In this work, signal system call is added to permit users to register handler for signals (SIGFPE, SIGSEGV)

Signal system call implementation:

1. To support custom handler for signals, the proc structure is modified to include data structure. This stores user-defined handlers for signals -0 for SIGFPE, 1 for SIGSEGV(proc.h)
2. When the process is created, allocproc() is called to initialize process state. The signal handler array is initialized to -1 to indicate no handlers are registered.
3. Signal number is added for signal system call(syscall.h)
4. The stub for signal system call is added(Usys.S)
5. Signal system call is declared and added to syscalls vector which points to code to run.(syscall.c)
6. Signal system call is defined in sysproc.c. It fetches parameters passed from stack - signum, user handler base address(trapframe of process) using function in syscall.c. The function registers handler defined in proc structure for corresponding signal. The system call returns -1 if user-defined signal handler is not supported or if registration fails. Upon success it returns handler base address.
7. Finally signal system call interface is provided for users(user.h).A new type sighandler_t is added(types.h) to specify type for handler function.     

Program Flow for signals:
When user calls signal system call, an interrupt is raised and interrupt number, T_SYSCALL, is placed in %eax register (Usys.c). syscall function is called where based on interrupt number the instruction pointer is changed to point to routine. signal function is executed and returns back to program.

Trap and Custom handler execution:

1. Signals - SIGFPE, SIGSEGV are defined in traps.h
2. The trapframe in process contains trap number when trap occurs. Based on trap number, user-defined handler base address is obtained. A new frame is opned. Current value instruction pointer(eip) is stored in top of stack(esp). eip is made to point handler. Once the handler is executed, it returns, pops stack content and execute interrupted instruction. [trap(), callUserHandler() in trap.c]

User process: [foo.c]

Input - Two numbers for division
The process generates segmentation fault.
SIGFPE is handled by user. Upon SIGSEGV, the process exits.
If the handlers do not handle or exit from process, it returns to the interrupted instruction. This verifies the validity of trap handler switching in kernel side.

Output:

/usr/share/bin/qemu-system-x86_64 -serial mon:stdio -hdb fs.img xv6.img -smp 2 -m 512 
xv6...
cpu1: starting
cpu0: starting
init: starting sh
$ foo 12 0  

Signal system call return value for SIGFPE: 0
Signal system call return value for SIGEGV: 62
Divide by Zero Error... 

Division result: 0

Segmentation fault..Exiting from program..
$ foo 12 2

Signal system call return value for SIGFPE: 0
Signal system call return value for SIGEGV: 62
Division result: 6

Segmentation fault..Exiting from program..

