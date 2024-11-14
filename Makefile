# Compiler and linker settings
CXX = x86_64-elf-g++
AS = nasm
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -mcmodel=large -mno-red-zone
LDFLAGS = -ffreestanding -O2 -nostdlib -z max-page-size=0x1000
ASFLAGS = -f elf64

# Source files
SOURCES_ASM = src/boot.asm
SOURCES_CPP = src/kernel.cpp

# Object files
OBJECTS = $(SOURCES_ASM:.asm=.o) $(SOURCES_CPP:.cpp=.o)

# Output files
KERNEL = myos.bin
ISO = myos.iso

.PHONY: all clean run

all: $(ISO)

$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/
	cp grub.cfg isodir/boot/grub/
	grub-mkrescue -o $(ISO) isodir

$(KERNEL): $(OBJECTS)
	$(CXX) -T src/linker.ld -o $@ $(LDFLAGS) $(OBJECTS) -lgcc

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO)

clean:
	rm -f $(OBJECTS) $(KERNEL) $(ISO)
	rm -rf isodir