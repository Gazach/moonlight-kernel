ISO_DIR = iso
ISO_NAME = moonlight.iso

ASM = nasm
CC = clang
LD = ld.lld

CFLAGS = --target=i386-pc-none-elf \
         -ffreestanding \
         -nostdlib \
         -fno-builtin \
         -fno-stack-protector
		 
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

OBJS = boot.o kernel.o

all: $(ISO_NAME)

boot.o: boot/boot.asm
	$(ASM) $(ASMFLAGS) $< -o $@

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(ISO_NAME): kernel.elf
	mkdir -p $(ISO_DIR)/boot/grub
	cp kernel.elf $(ISO_DIR)/boot/kernel.elf
	printf 'menuentry "Moonlight Kernel" {\n\tmultiboot2 /boot/kernel.elf\n\tboot\n}\n' > $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) $(ISO_DIR)

run: $(ISO_NAME)
	qemu-system-i386 -cdrom $(ISO_NAME)

clean:
	rm -rf $(ISO_DIR)
	rm -f *.o
	rm -f kernel.elf
	rm -f $(ISO_NAME)

.PHONY: all run clean