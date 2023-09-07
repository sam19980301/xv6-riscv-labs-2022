# Lab: Using gdb
> Looking at the backtrace output, which function called syscall?
- `usertrap()` at `kernel/trap.c:67`

> What is the value of p->trapframe->a7 and what does that value represent?
- `p->trapframe->a7` is `7`, which refers to SYS_exec

> What was the previous mode that the CPU was in?
- `$sstatus` is `0x200000022` with SPP(8-th) bit set to `0`, so previous mode is user mode.

> Write down the assembly instruction the kernel is panicing at. Which register corresponds to the varialable num?
```
scause 0x000000000000000d
sepc=0x0000000080001ff8 stval=0x0000000000000000
panic: kerneltrap
```
- According to the messge above and assembly code `kernel/kernel.asm`, error occurs at `lw a3,0(zero)` which corresponds to the source code `num = * (int *) 0`

> Why does the kernel crash?
- According to Fig 3.3 in course reference textbook, kernel crashes because of accessing an unused `0` virtual address.

> What is the name of the binary that was running when the kernel paniced? What is its process id (pid)?
- Kernel paniced at `initcode` binary code, with `pid` 1.
