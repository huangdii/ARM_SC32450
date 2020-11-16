#
# Source files that must be built to ARM mode.
#
export MAIN_SRC = $(shell pwd)/main.c
export INCLUDE = $(shell pwd)

SUBDIRS = ../../FreeRTOS/Demo/OMAP3_BeagleBoard_GCC
     
.PHONY: subdirs $(SUBDIRS)
     
subdirs: $(SUBDIRS)
     
$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C $(SUBDIRS) clean
