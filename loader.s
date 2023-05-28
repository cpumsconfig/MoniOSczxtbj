.set MAGIC, 0x1badb002;GRUB魔术块
.set FLAGS, (1<<0 | 1<<1);GRUB标志块
.set CHECKSUM, -(MAGIC + FLAGS);校验块

.section .multboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM
.section .text
.extern kernel_main;导入kernel_main
.extern system_constructors;导入系统构造函数
.global laoder

loader:
	mov $kernel_stack, %esp
	call system_constructors
	push %eax
	push %ebx
	call kernel_main

stop:
	cli
	hlt
	jmp stop

.section .bss
.space 2*1024*1024
kernel_stack:
