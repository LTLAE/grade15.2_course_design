#
# Makefile
#-L/home/gec/freetype-2.12.1/tmp/lib -I/home/gec/freetype-2.12.1/tmp/include/freetype2
# CC = arm-linux-gcc
CC = arm-linux-gnueabi-gcc
LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= ${shell pwd}
CFLAGS ?= -O3 -g0   -I$(LVGL_DIR)/ -I$(LVGL_DIR)/usrCode -I$(LVGL_DIR)/usrLib -Wall  -std=gnu99
LDFLAGS ?= -L$(LVGL_DIR)/usrLib -lm -lfreetype -lpthread
BIN = s4uclock


#Collect the files to compile
USRCODESRC = ${shell find $(LVGL_DIR)/usrCode -name '*.c'}
MAINSRC = ./main.c

include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

CSRCS +=$(LVGL_DIR)/mouse_cursor_icon.c 

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))
USRCODEOBJ=$(USRCODESRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC) $(USRCODESRC)
OBJS = $(AOBJS) $(COBJS)

## MAINOBJ -> OBJFILES


all: default

%.o: %.c
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"
    
default: $(AOBJS) $(COBJS) $(MAINOBJ) $(USRCODEOBJ)
	$(CC) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(USRCODEOBJ) $(LDFLAGS)

clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ) $(USRCODEOBJ)

