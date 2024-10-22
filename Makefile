ISO_FILE = $(KERNEL_DIR)/dyos.iso
START_OBJ = $(KERNEL_DIR)/start.o
KERNEL_BIN = $(KERNEL_DIR)/kernel.bin
BOOTLOADER_FILE = grub.cfg

MOUNTPOINT = /mnt
ISO_DIR = ./iso
LOG_DIR = ./log
BOOT_DIR = $(ISO_DIR)/boot
BOOTLOADER_DIR = $(BOOT_DIR)/grub
KERNEL_DIR = ./src/kernel
BUILD_DIR = ./build

GCC_ARGUMENTS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -m32 -fno-PIC
QEMU_ARGUMENTS = -accel tcg,thread=single -cpu core2duo -m 128 -no-reboot -cdrom $(ISO_FILE) -serial stdio -smp 1 -usb -vga std -d int,cpu_reset,guest_errors -D $(LOG_DIR)/qemu_debug.log -boot d

SOURCES = main.c screen.c gdt.c idt.c isr.c
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

all: debug

# Assemble the assembly
$(START_OBJ): ./src/start.asm $(BUILD_DIR) $(BOOTLOADER_DIR)
	nasm -f elf32 $< -o $@

# Link the C code to the assembly
$(KERNEL_BIN): $(START_OBJ) $(OBJECTS)
	ld -m elf_i386 -T ./src/link.ld -o $@ $^

# Compile the C files
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c | $(BUILD_DIR) | $(BOOTLOADER_DIR)
	gcc $(GCC_ARGUMENTS) -I $(KERNEL_DIR)/includes -c -o $@ $<

# Create Build direction
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create GRUB base Config File
$(BOOTLOADER_DIR):
	mkdir -p $(BOOTLOADER_DIR)
	echo 'menuentry "Boot Dynos" {' > $(BOOTLOADER_DIR)/$(BOOTLOADER_FILE)
	echo '    multiboot /boot/kernel.bin' >> $(BOOTLOADER_DIR)/$(BOOTLOADER_FILE)
	echo '    boot' >> $(BOOTLOADER_DIR)/$(BOOTLOADER_FILE)
	echo '}' >> $(BOOTLOADER_DIR)/$(BOOTLOADER_FILE)

# Burn ISO image
burn: $(KERNEL_BIN)
	@echo "Burning ISO Image..."
	cp $(KERNEL_BIN) $(BOOT_DIR)
	genisoimage -o $(ISO_FILE) -R -J $(ISO_DIR)

# Mount grub into the ISO 
grub-mount: burn
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

# Launch Qemu
debug: grub-mount
	qemu-system-i386 $(QEMU_ARGUMENTS)

# Clean build and log files
clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BOOT_DIR)/kernel.bin
	rm -rf $(ISO_FILE)
	rm -rf $(LOG_DIR)/qemu_debug.log

# Mount the iso into a mountpoint for prior test purpose
mount-test:
	sudo mount -o loop $(ISO_FILE) $(MOUNTPOINT)

# Check ISO File sanity 
iso-sanity:
	file $(ISO_FILE)

# Unmount the iso
end-mount-test:
	sudo umount $(MOUNTPOINT)