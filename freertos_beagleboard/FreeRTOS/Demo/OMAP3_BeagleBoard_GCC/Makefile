#/*
# * FreeRTOS Kernel V10.1.1
# * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# *
# * Permission is hereby granted, free of charge, to any person obtaining a copy of
# * this software and associated documentation files (the "Software"), to deal in
# * the Software without restriction, including without limitation the rights to
# * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# * the Software, and to permit persons to whom the Software is furnished to do so,
# * subject to the following conditions:
# *
# * The above copyright notice and this permission notice shall be included in all
# * copies or substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# *
# * http://www.FreeRTOS.org
# * http://aws.amazon.com/freertos
# *
# * Created by YUNG KI HONG on 2018-12-13
# * Copyright (c) 2015 guileschool.com All rights reserved.
# *
# * 1 tab == 2 spaces!
# */
.EXPORT_ALL_VARIABLES:
	
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

#
# build option
#
USE_THUMB_MODE=YES  
DEBUG=-g
OPTIM=-O0
RUN_MODE=RUN_FROM_RAM
LDSCRIPT=omap3-ram.ld

#
# CFLAGS common to both the THUMB and ARM mode builds
#
CFLAGS=-Wall -Wextra -D $(RUN_MODE) -DGCC_OMAP3_BeagleBoard -I$(INCLUDE) -I./ -I../../Source/include \
	  -I./Common/include -I../../Source/portable/GCC/OMAP3_BeagleBoard $(DEBUG) -mcpu=cortex-a8  \
	  -Wcast-align $(OPTIM) -Wstrict-prototypes -fomit-frame-pointer -fno-strict-aliasing \
	  -fno-common -fno-builtin -mfloat-abi=softfp \
	  -fno-dwarf2-cfi-asm -fno-builtin -mapcs -pipe -Wno-trigraphs

ifeq ($(USE_THUMB_MODE),YES)
	CFLAGS += -mthumb-interwork -D THUMB_INTERWORK
	THUMB_FLAGS=-mthumb
endif

LINKER_FLAGS=--cref -Bstatic -lm -lc -lgcc --entry main -T$(LDSCRIPT) -M -Map=rtosdemo.map
LINKER_FLAGS+=-L/opt/gcc-linaro-6.5.0-2018.12-i686_arm-eabi/arm-eabi/libc/usr/lib -L/opt/gcc-linaro-6.5.0-2018.12-i686_arm-eabi/lib/gcc/arm-eabi/6.5.0

#
# Source files that can be built to THUMB mode.
#
THUMB_SRC = \
$(MAIN_SRC) \
ParTest/ParTest.c \
../../Source/tasks.c \
../../Source/croutine.c \
../../Source/queue.c \
../../Source/list.c \
../../Source/timers.c \
../../Source/event_groups.c \
../../Source/portable/MemMang/heap_4.c

#
# Source files that must be built to ARM mode.
#
ARM_SRC = \
../../Source/portable/GCC/OMAP3_BeagleBoard/portISR.c \
../../Source/portable/GCC/OMAP3_BeagleBoard/port.c \
myserial.c \
serial/serial.c \
serial/syscall.c \
Exception.c \
serial/serialISR.c

#
# Assembly
#
CRT0_SRC=boot.s random.s libs.s

#
# Define all object files.
#
THUMB_OBJ = $(THUMB_SRC:.c=.o)
ARM_OBJ = $(ARM_SRC:.c=.o)
CRT0_OBJ = $(CRT0_SRC:.s=.o)

rtosdemo.bin : rtosdemo.elf
	$(OBJCOPY) --gap-fill=0xff -O binary rtosdemo.elf $@
	$(OBJDUMP) -d $< > rtosdemo.dis
	cp rtosdemo.bin /tftpboot
	echo -en '\007'

# mkimage -A arm -T firmware -C none -O u-boot \
#          -a 0x80100000 -e 0x80100000 \
#          -n "FreeRTOS for BeagleBoard Rev-C5 board" -d rtosdemo.bin rtosdemo.img

rtosdemo.elf : $(ARM_OBJ) $(THUMB_OBJ) $(CRT0_OBJ)
	$(LD) -o rtosdemo.elf $(ARM_OBJ) $(THUMB_OBJ) $(CRT0_OBJ) $(LINKER_FLAGS)

$(THUMB_OBJ) : %.o : %.c
	$(CC) -c $(THUMB_FLAGS) $(CFLAGS) $< -o $@

$(ARM_OBJ) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(CRT0_OBJ) : %.o : %.s
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	rm -f $(ARM_OBJ) $(THUMB_OBJ) $(CRT0_OBJ)
	rm -f rtosdemo.*