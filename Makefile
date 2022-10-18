prefix = arm-none-eabi

build_dir = build
src_dir = src

# project name & config 
target = template
debug = 1
opt = -Og

# sources 
asm_sources +=
c_sources += src/main.c
c_sources += $(wildcard src/periph/src/*.c)

# defines
defs += -DUSE_STDPERIPH_DRIVER
defs += -DSTM32F10X_MD

# includes 
includes += -Isrc
includes += -Isrc/periph/inc

# include platform support file
include src/platform/makefile