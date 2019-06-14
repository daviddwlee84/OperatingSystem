# XV6 Source Code Reading -- Interrupt and System Call

## Requirement

Code to read

* Bootloader
  * [ ] `bootasm.S` (p.91)
  * [ ] `bootmain.c` (p.92)
* Entering xv6
  * [ ] `entry.S` (p.10)
  * [ ] `entryother.S` (p.11)
  * [ ] `main.c` (p.12)
* System calls (and interrupt)
  * [ ] `traps.h` (p.32)
  * [ ] `vectors.pl` (p.32)
  * [ ] `trapasm.S` (p.33)
  * [ ] `trap.c` (p.33)
  * [ ] `syscall.h` (p.35)
  * [ ] `syscall.c` (p.35)
  * [ ] `sysproc.c` (p.37)

## Resources

### Instruction

#### Interrupt

* cli - Clear Interrupt Flag
  * [x86 Instruction Set Reference - CLI](https://c9x.me/x86/html/file_module_x86_id_31.html)
  * [CLI — Clear Interrupt Flag](https://www.felixcloutier.com/x86/cli)
* sti - Set Interrupt Flag
  * [STI — Set Interrupt Flag](https://www.felixcloutier.com/x86/sti)
* int
  * [INT n/INTO/INT3/INT1 — Call to Interrupt Procedure](https://www.felixcloutier.com/x86/intn:into:int3:int1)
  * [wiki - INT (x86 instruction)](https://en.wikipedia.org/wiki/INT_(x86_instruction))
