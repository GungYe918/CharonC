ARCH     ?= x86_64
SYSROOT   := $(CURDIR)/sysroot
KERNELDIR := kernel
BUILDDIR  := $(KERNELDIR)/build
OUTFILE   := ../esp/kernel/kernel.elf

ifeq ($(ARCH), x86_64)
	CC      = $(SYSROOT)/bin/x86_64-charonos-gcc
	LOCORE  = $(KERNELDIR)/amd64/locore.S
	LINKER  = $(KERNELDIR)/amd64/linker.ld
	CFLAGS  = -ffreestanding -mno-red-zone -Wall -Wextra \
	          --sysroot=$(SYSROOT) -I$(SYSROOT)/usr/include -I$(KERNELDIR) \
	          -fno-pie -no-pie -I/kernel/Lib -mcmodel=kernel -g -O0

else ifeq ($(ARCH), aarch64)
	CC      = aarch64-none-elf-gcc
	LOCORE  = $(KERNELDIR)/arm64/locore.S
	LINKER  = $(KERNELDIR)/arm64/linker.ld
	CFLAGS  = -ffreestanding -Wall -Wextra \
	          --sysroot=$(SYSROOT) -I$(SYSROOT)/usr/include -I$(KERNELDIR) \
	          -fno-pie -no-pie -I/kernel/Lib -g -O0

else
$(error Unsupported ARCH: $(ARCH))
endif

#CFLAGS  = -ffreestanding -mno-red-zone -Wall -Wextra \
#          --sysroot=$(SYSROOT) -I$(SYSROOT)/usr/include -I$(KERNELDIR) \
#          -fno-pie -no-pie -I/kernel/Lib	-mcmodel=kernel	-g	-O0

LDFLAGS = -nostdlib -T $(LINKER) -fno-pie -no-pie	-z max-page-size=0x1000	-g

# BUild Files
Lib = paint.o ../sys/dev/fb/fb.o	../sys/dev/vt/vt_font.o	../sys/dev/vt/vt.o

# 자동으로 전체 OBJS를 구성
OBJS = $(BUILDDIR)/locore.o $(BUILDDIR)/init.o \
       $(addprefix $(BUILDDIR)/, $(Lib))

all: $(OUTFILE)

$(OUTFILE): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^

# C 파일 빌드 규칙
$(BUILDDIR)/%.o: $(KERNELDIR)/Lib/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# ASM 파일 빌드 규칙
$(BUILDDIR)/%.o: $(KERNELDIR)/Lib/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# init.c 빌드
$(BUILDDIR)/init.o: $(KERNELDIR)/init/init.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# locore.S 빌드
$(BUILDDIR)/locore.o: $(LOCORE)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILDDIR) $(OUTFILE)

.PHONY: all clean
