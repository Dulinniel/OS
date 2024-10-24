ISO_FILE = $(BUILD_DIR)/dyos.iso
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
LINKER_SCRIPT = ./src/link.ld
BOOTLOADER_FILE = grub.cfg

MOUNTPOINT = /mnt
ISO_DIR = ./iso
LOG_DIR = ./log
BOOT_DIR = $(ISO_DIR)/boot
BOOTLOADER_DIR = $(BOOT_DIR)/grub
KERNEL_DIR = ./src/kernel
BUILD_DIR = ./build

CC = gcc
LD = ld
ASM = nasm

NASM_ARGUMENTS = -f elf32
LD_ARGUMENTS = -m elf_i386 -T $(LINKER_SCRIPT)
CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -m32 -fno-PIC
QEMU_ARGUMENTS = -accel tcg,thread=single -cpu core2duo -m 128 -no-reboot -s -S -cdrom $(ISO_FILE) -serial stdio -smp 1 -usb -vga std -d int,cpu_reset,guest_errors -D $(LOG_DIR)/qemu_debug.log -boot d

C_SOURCES = $(wildcard $(KERNEL_DIR)/*.c)
ASM_SOURCES = $(wildcard ./src/*.asm)
C_OBJECTS = $(C_SOURCES:$(KERNEL_DIR)/%.c=$(BUILD_DIR)/c/%.o)
ASM_OBJECTS = $(ASM_SOURCES:./src/%.asm=$(BUILD_DIR)/asm/%.o)

all: debug

$(BUILD_DIR)/c/%.o: $(KERNEL_DIR)/%.c | build-config
	$(CC) $(CFLAGS) -I $(KERNEL_DIR)/includes -c -o $@ $<

$(BUILD_DIR)/asm/%.o: ./src/%.asm | build-config
	$(ASM) $(NASM_ARGUMENTS) $< -o $@

$(KERNEL_BIN): $(ASM_OBJECTS) $(C_OBJECTS)
	$(LD) $(LD_ARGUMENTS)  -o $@ $^

build-config:
	mkdir -p $(BUILD_DIR)/c
	mkdir -p $(BUILD_DIR)/asm

bootloader-config:
	mkdir -p $(BOOTLOADER_DIR)
	echo 'menuentry "Boot Dynos" {' > $(BOOTLOADER_DIR)/$(BOOTLOADER_FILE)
	echo '    multiboot /boot/kernel.bin' >> $(BOOTLOADER_DIR)/$(BOOTLOADER_FILE)
	echo '    boot' >> $(BOOTLOADER_DIR)/$(BOOTLOADER_FILE)
	echo '}' >> $(BOOTLOADER_DIR)/$(BOOTLOADER_FILE)

burn: $(KERNEL_BIN) bootloader-config
	cp $(KERNEL_BIN) $(BOOT_DIR)
	genisoimage -o $(ISO_FILE) -R -J $(ISO_DIR)

grub-mount: burn
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

debug: grub-mount
	qemu-system-i386 $(QEMU_ARGUMENTS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BOOT_DIR)/kernel.bin
	rm -rf $(ISO_FILE)
	rm -rf $(LOG_DIR)/qemu_debug.log

mount-test:
	sudo mount -o loop $(ISO_FILE) $(MOUNTPOINT)

iso-sanity:
	file $(ISO_FILE)

end-mount-test:
	sudo umount $(MOUNTPOINT)

attach:
	gdb
	target remote localhost:1234