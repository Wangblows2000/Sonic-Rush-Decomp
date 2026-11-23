# Sonic Rush DC game. Needs: . /opt/toolchains/dc/kos/environ.sh
TARGET = srdc.elf
OBJS = src/main.o engine/render.o engine/input.o engine/audio.o engine/level.o engine/loading.o engine/pak.o engine/frameprof.o \
       $(patsubst %.c,%.o,$(wildcard player/dcp_*.c)) model/model.o \
       $(patsubst %.c,%.o,$(wildcard objects/obj_*.c)) \
       $(patsubst %.c,%.o,$(wildcard ui/ui_*.c)) \
       sound/snd_bank.o sound/snd_drv.o sound/snd_nns.o sound/sound.o sound/sound_hooks.o sound/snd_out_kos.o sound/bgm.o \
       $(patsubst %.c,%.o,$(filter-out $(BOSS_WIP),$(wildcard boss/boss*.c))) \
       $(patsubst %.c,%.o,$(wildcard menus/mn_*.c)) save/save.o save/save_vmu.o \
       $(patsubst %.c,%.o,$(wildcard special/sp_*.c)) \
       $(patsubst %.c,%.o,$(wildcard cutscene/cs_*.c)) demo/demo.o \
       camera/camera.o zonefx/zonefx.o
KOS_CFLAGS += -O2 -fwrapv -I../../tools/dreamcast/stages
# measurement build: make MEMTRACK=1 (engine/memtrack.c: live heap blocks per caller, compared
# between visits of the scene named in /cd/debug/memtrack.txt); clean first, as for any flag change
ifdef MEMTRACK
OBJS += engine/memtrack.o
MEMTRACK_LDFLAGS = -Wl,--wrap=malloc,--wrap=free,--wrap=calloc,--wrap=realloc,--wrap=memalign
endif

all: $(TARGET)
include $(KOS_BASE)/Makefile.rules
clean:
	-rm -f $(TARGET) $(OBJS) engine/memtrack.o
$(TARGET): $(OBJS)
	kos-cc $(MEMTRACK_LDFLAGS) -o $(TARGET) $(OBJS) -lm
