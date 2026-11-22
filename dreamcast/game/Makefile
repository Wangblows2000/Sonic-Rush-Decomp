# Sonic Rush DC game. Needs: . /opt/toolchains/dc/kos/environ.sh
TARGET = srdc.elf
OBJS = src/main.o engine/render.o engine/input.o engine/audio.o engine/level.o \
       player/dcp_math.o player/dcp_probe.o player/dcp_object.o player/dcp_player.o player/dcp_states.o model/model.o
KOS_CFLAGS += -O2 -fwrapv -I../../tools/dreamcast/stages

all: $(TARGET)
include $(KOS_BASE)/Makefile.rules
clean:
	-rm -f $(TARGET) $(OBJS)
$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -lm
