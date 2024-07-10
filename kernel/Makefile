TARGET = kernel.elf

OBJS = main.o 

DEPENDS = $(join $(dir $(OBJS)),$(addprefix .,$(notdir $(OBJS:.o=.d))))

CFLAGS   += -Wall -g -ffreestanding -mno-red-zone
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
            -fno-exceptions -fno-rtti -std=c++17
LDFLAGS  += --entry KernelMain -z norelro --Ttext 0x100000	-L../sysroot/usr/lib

CC = ../sysroot/bin/x86_64-charonos-gcc

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) $(DEPENDS)

$(TARGET): $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o $(TARGET) $(OBJS) -lc

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm Makefile
	nasm -f elf64 -o $@ $<

.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

-include $(DEPENDS)
