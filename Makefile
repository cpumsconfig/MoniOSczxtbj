GCCPARAMS = -m32 -W -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPPARAMS = --32
LDPARAMS = -melf_i386
GCC = g++
ASM = as
LINKER = ld
CFLAGS = -o $@ -c $<
ASMFLAGS = -o $@ $<
LINKERFLAGS = -T $< -o $@ 

objects = boot.o kernel.o

%.o: %.c
	$(GCC) $(GCCPARAMS) $(CFLAGS)
	
%.o: %.s
	$(ASM) $(ASPPARAMS) $(ASMFLAGS)
	
kernel_lab.bin: linker.ld $(objects)
	$(LINKER) $(LINKERFLAGS) $(objects)
	
all: kernel_lab.bin
	echo "build successed"
clean: 
	rm -rf *.o
	rm -rf *.out
	rm -rf iso
	rm -rf *.iso
	rm -rf *.bin
	
rebuild: clean all
	echo "rebuild"
	
install: kernel_lab.bin
	sudo cp $< /boot/kernel_lab.bin

kernel_lab.iso: rebuild
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kernel_lab.bin iso/boot/
	cp boot/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso
	rm -rf iso
	
kernel_vm: kernel_lab.iso
	(killall virtualbox) || true
	virtualbox -startvm "kernel_lab" &
