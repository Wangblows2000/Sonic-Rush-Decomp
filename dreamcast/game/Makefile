# Sonic Rush DC game. Needs: . /opt/toolchains/dc/kos/environ.sh
TARGET = srdc.elf
OBJS = src/main.o engine/render.o engine/input.o engine/audio.o engine/level.o \
       $(patsubst %.c,%.o,$(wildcard player/dcp_*.c)) model/model.o \
       $(patsubst %.c,%.o,$(wildcard objects/obj_*.c)) \
       $(patsubst %.c,%.o,$(wildcard ui/ui_*.c)) \
       sound/snd_bank.o sound/snd_drv.o sound/snd_nns.o sound/sound.o sound/sound_hooks.o sound/snd_out_kos.o \
       $(patsubst %.c,%.o,$(filter-out $(BOSS_WIP),$(wildcard boss/boss*.c))) \
       $(patsubst %.c,%.o,$(wildcard menus/mn_*.c)) save/save.o save/save_vmu.o \
       $(patsubst %.c,%.o,$(wildcard special/sp_*.c))
KOS_CFLAGS += -O2 -fwrapv -I../../tools/dreamcast/stages

all: $(TARGET)
include $(KOS_BASE)/Makefile.rules
clean:
	-rm -f $(TARGET) $(OBJS)
$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -lm
