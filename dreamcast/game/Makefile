# Sonic Rush DC game. Needs: . /opt/toolchains/dc/kos/environ.sh
TARGET = srdc.elf
OBJS = src/main.o engine/render.o engine/input.o engine/audio.o engine/level.o \
       player/dcp_math.o player/dcp_probe.o player/dcp_object.o player/dcp_player.o player/dcp_states.o model/model.o \
       objects/obj_anim.o objects/obj_colmap.o objects/obj_draw.o objects/obj_enemy.o objects/obj_gimmicks.o \
       objects/obj_manager.o objects/obj_player.o objects/obj_rect.o objects/obj_ring.o
KOS_CFLAGS += -O2 -fwrapv -I../../tools/dreamcast/stages

all: $(TARGET)
include $(KOS_BASE)/Makefile.rules
clean:
	-rm -f $(TARGET) $(OBJS)
$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -lm
