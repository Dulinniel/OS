FILENAME = dyos.iso
ISO_FOLDER = ./iso
KERNEL_DIR = ./src/kernel
BUILD_DIR = ./build
GCC_ARGUMENTS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -m32 -fno-PIC

all: debug

build-start:
	nasm -f elf32 ./src/start.asm -o $(BUILD_DIR)/start.o

link-start: kernel_compile
	ld -m elf_i386 -T ./src/link.ld -o $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/start.o $(BUILD_DIR)/main.o ${BUILD_DIR}/screen.o $(BUILD_DIR)/gdt.o

kernel_compile: build-start
	gcc $(GCC_ARGUMENTS) -I ./src/kernel/includes -c -o $(BUILD_DIR)/main.o src/kernel/main.c
	gcc $(GCC_ARGUMENTS) -I ./src/kernel/includes -c -o $(BUILD_DIR)/screen.o src/kernel/screen.c
	gcc $(GCC_ARGUMENTS) -I ./src/kernel/includes -c -o $(BUILD_DIR)/gdt.o src/kernel/gdt.c

burn: link-start
	cp $(BUILD_DIR)/kernel.bin $(ISO_FOLDER)/boot
	genisoimage -o $(BUILD_DIR)/$(FILENAME) -R -J $(ISO_FOLDER)

grub-mount: burn
	grub-mkrescue -o $(BUILD_DIR)/$(FILENAME) $(ISO_FOLDER)

debug: grub-mount
	qemu-system-i386                               							\
  -accel tcg,thread=single                       							\
  -cpu core2duo                                  							\
  -m 128                                         							\
  -no-reboot                                     							\
  -cdrom $(BUILD_DIR)/$(FILENAME) 														\
  -serial stdio                                  							\
  -smp 1                                         							\
  -usb                                           							\
  -vga std 																										\
  -d int,cpu_reset,guest_errors																\
  -D log/qemu_debug.log 																			\
  -boot d

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(ISO_FOLDER)/boot/kernel.bin

mount-test:
	sudo mount -o loop $(BUILD_DIR)/$(FILENAME) /mnt

end-mount-test:
	sudo umount /mnt