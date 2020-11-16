##########[Embedded C test firmware Makefile]##############
#
# NAME : Makefile - S3C2450 test Firmware Makefile
# Brief history
#----------------------------------------------------------
#
#	2015.08.10, Seonghye : Modified
#
###########################################################

.EXPORT_ALL_VARIABLES:

## If you want to change path, modify here

TOPDIR =$(PWD)
TOOLPATH = /opt/CodeSourcery/Sourcery_G++_Lite

CC = $(TOOLPATH)/bin/arm-none-eabi-gcc
LD = $(TOOLPATH)/bin/arm-none-eabi-ld
OBJCOPY	= $(TOOLPATH)/bin/arm-none-eabi-objcopy
OBJDUMP	= $(TOOLPATH)/bin/arm-none-eabi-objdump

LIBCDIR =$(TOOLPATH)/arm-none-eabi/lib
LIBGCCDIR =$(TOOLPATH)/lib/gcc/arm-none-eabi/4.5.2
LIBC =$(TOOLPATH)/arm-none-eabi/lib/libc.a
LIBCINCDIR =$(TOOLPATH)/arm-none-eabi
LIBGCC = $(TOOLPATH)/lib/gcc/arm-none-eabi/4.5.2/libgcc.a

OUTDIR      = ucos2_data
OBJDIR      = $(OUTDIR)/ObjectCode
APPDIR      = app
BSPDIR      = bsp
KERDIR      = kernel
INCDIR      = include

UCOSSRC     = $(KERDIR)/uCOS_II.c
BSPSRC      = $(BSPDIR)/exception.c\
							$(BSPDIR)/libc.c\
							$(BSPDIR)/Uart.c\
							$(BSPDIR)/syscall.c\
              $(BSPDIR)/timer.c\
              $(BSPDIR)/lcddrv.c\
              $(BSPDIR)/PC.c\
              $(BSPDIR)/os_dbg_r.c\
              $(BSPDIR)/malloc_lock.c\
              $(BSPDIR)/os_cpu_c.c
              
BSPASRC 	= $(BSPDIR)/s3c2450_startup.S\
              $(BSPDIR)/libs.S\
              $(BSPDIR)/random.S\
              $(BSPDIR)/os_cpu_a.S

APPSRC 		= $(APPDIR)/01_TASKMAN/01_TASKMAN.c
			# 01_TASKMAN/01_TASKMAN.c
			# 02_TIME/02_TIME.c
			# 03_CRITICAL/03_CRITICAL.c
			# 04_STAT/04_STAT.c
			# 05_MEMORY/05_MEMORY.c
			# 06_SEM/06_SEM.c
			# 07_MUTEX/07_MUTEX.c
			# 08_FLAG/08_FLAG.c
			# 09_MBOX/09_MBOX.c
			# 10_QUE/10_QUE.c
			# 11_DEADLOCK/11_DEADLOCK.c
			
OBJS        =  $(BSPSRC:.c=.o) $(BSPASRC:.S=.o)
OBJS    	+= $(UCOSSRC:.c=.o)
OBJS    	+= $(APPSRC:.c=.o)

#### Option Definition ####
INCLUDE	=  -nostdinc -I$(BSPDIR) -I$(TOPDIR)/include -I$(LIBCINCDIR)/include -I$(LIBGCCDIR)/include

CFLAGS	+= $(INCLUDE) -g -Wall -Wstrict-prototypes -Wno-trigraphs -O0
CFLAGS	+= -fno-strict-aliasing -fno-common -pipe
CFLAGS += -D_GCC_LIMITS_H_ -march=armv5te -mtune=arm9e -fno-builtin -mapcs
CFLAGS += -D_LCD_480x272
#CFLAGS  +=  -D__XDEF_REPORT_STAT

LDFLAGS	= --cref -Bstatic -nostdlib -nostartfiles -p -EL -T S3C2450-RAM.ld -Map \
			$(OUTDIR)/MDS2450.map 

OCFLAGS = -O binary -R .note -R .comment -S

2450TEST = MDS2450.bin

%.o:%.S
	$(CC) -c $(CFLAGS) -o $@ $<

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(2450TEST)

$(2450TEST) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(OUTDIR)/MDS2450 $(OBJS) $(LIBC) $(LIBGCC) \
	-I$(LIBGCCDIR)/lib -L$(LIBCDIR) -L$(LIBC) -L$(LIBGCCDIR) -lc

	$(OBJCOPY) $(OCFLAGS) $(OUTDIR)/MDS2450 $(OUTDIR)/$@
	cp $(OUTDIR)/$@ /tftpboot

clean:
	rm -f $(APPDIR)/*.o $(APPDIR)/*/*.o 
	rm -f $(BSPDIR)/*.o $(BSPDIR)/drv/*.o 
	rm -f $(KERDIR)/*.o 
	rm -f $(OUTDIR)/MDS2450*

dep:
	$(CC) -M $(INCLUDE) $(UCOSSRC) $(BSPSRC) $(APPSRC) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif					
