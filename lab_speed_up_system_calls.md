# Lab: Speed up system calls
> Which other xv6 system call(s) could be made faster using this shared page? Explain how.
- Any system call that needs to transfer the data between the user and kernel space could be faster. Sharing such a page saves time doing `copyin` and `copyout` operations.
