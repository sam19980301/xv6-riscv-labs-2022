# Lab: RISC-V assembly
> Which registers contain arguments to functions? For example, which register holds 13 in main's call to printf?
- `a0` to `a7` and `fa0` to `fa7` contain arguments to functions, and register `a2` holds the value 13.

> Where is the call to function f in the assembly code for main? Where is the call to g?
- Both are inlined via compiler optimization

> At what address is the function printf located?
- `0x630`

> What value is in the register ra just after the jalr to printf in main?
- `0x34` + 4 = `0xx38`

> Run the following code. What is the output? The output depends on that fact that the RISC-V is little-endian. If the RISC-V were instead big-endian what would you set i to in order to yield the same output? Would you need to change 57616 to a different value?
```
unsigned int i = 0x00646c72;
printf("H%x Wo%s", 57616, &i);
```
- HE110 World
- If RISC-V were big-endian machine, set i to `0x726c6400` and leave 57616 unchanged.

> In the following code, what is going to be printed after 'y='? Why does this happen?
```
printf("x=%d y=%d", 3);
```
- 8229, which is an undefined value in register `a2` not initialized by `main`.
