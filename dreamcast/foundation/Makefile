# Sonic Rush Dreamcast port — Milestone 2 foundation.
# Needs a KallistiOS environment: source /opt/toolchains/dc/kos/environ.sh
TARGET = srdc_foundation.elf
OBJS = main.o

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean: rm-elf
	-rm -f $(OBJS)

rm-elf:
	-rm -f $(TARGET)

$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS)
