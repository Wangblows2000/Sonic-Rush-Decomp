# Native boss module (Dreamcast port). Targets:
#   make data       BYOR: build/dreamcast/boss/{bossdata.bin,assets.txt} (needs extract/eur, models converted)
#   make host-test  build + run the host tests (Egg Hammer on z13, Egg Turtle on z23)
#   make host-test1 / host-test2 / host-test3 / host-test4   one of them (Boss3: z33, Boss4: z43)
#   make dc         compile all sources with the KOS toolchain (kos-cc -Werror) into $(OUT)/dc/
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/boss
PLAYER_DIR = ../player
OBJ_DIR = ../objects
MODEL_DIR = ../model
PLAYER_SRCS = $(PLAYER_DIR)/dcp_math.c $(PLAYER_DIR)/dcp_probe.c $(PLAYER_DIR)/dcp_object.c \
              $(PLAYER_DIR)/dcp_player.c $(PLAYER_DIR)/dcp_states.c
# every object-module source except the KOS renderer
OBJ_SRCS = $(filter-out $(OBJ_DIR)/obj_draw.c,$(wildcard $(OBJ_DIR)/obj_*.c))
SRCS = boss_core.c boss_camera.c boss_model.c boss_effect.c boss1.c boss2.c boss3.c boss4.c
HDRS = boss.h boss_internal.h
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv
Z13 ?= $(ROOT)/build/dreamcast/stages/z13/level.bin
PLAYER_TABLES ?= $(ROOT)/build/dreamcast/player
OBJDATA ?= $(ROOT)/build/dreamcast/objects
SPRITES ?= $(ROOT)/build/dreamcast/sprites/bac
MODELS ?= $(ROOT)/build/dreamcast/models

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/boss/tools/bossdata.py --out $(abspath $(OUT))

Z23 ?= $(ROOT)/build/dreamcast/stages/z23/level.bin
Z33 ?= $(ROOT)/build/dreamcast/stages/z33/level.bin
Z43 ?= $(ROOT)/build/dreamcast/stages/z43/level.bin

$(OUT)/host/lib.stamp: $(SRCS) $(HDRS) $(PLAYER_SRCS) $(OBJ_SRCS) $(MODEL_DIR)/model.c test/stubs_objects.c
	rm -rf $(OUT)/host && mkdir -p $(OUT)/host/lib
	for f in $(SRCS); do gcc $(CFLAGS_HOST) -c $$f -o $(OUT)/host/lib/b_$$(basename $${f%.c}).o || exit 1; done
	for f in $(PLAYER_SRCS) $(OBJ_SRCS) $(MODEL_DIR)/model.c test/stubs_objects.c; do gcc -std=c99 -O2 -fwrapv -ffunction-sections -fdata-sections -c $$f -o $(OUT)/host/lib/x_$$(basename $${f%.c}).o || exit 1; done
	touch $@

$(OUT)/test_boss: $(OUT)/host/lib.stamp test/test_boss.c
	gcc $(CFLAGS_HOST) -c test/test_boss.c -o $(OUT)/host/test_boss.o
	gcc -Wl,--gc-sections -o $@ $(OUT)/host/lib/*.o $(OUT)/host/test_boss.o -lm

$(OUT)/test_boss2: $(OUT)/host/lib.stamp test/test_boss2.c
	gcc $(CFLAGS_HOST) -c test/test_boss2.c -o $(OUT)/host/test_boss2.o
	gcc -Wl,--gc-sections -o $@ $(OUT)/host/lib/*.o $(OUT)/host/test_boss2.o -lm

$(OUT)/test_boss3: $(OUT)/host/lib.stamp test/test_boss3.c
	gcc $(CFLAGS_HOST) -c test/test_boss3.c -o $(OUT)/host/test_boss3.o
	gcc -Wl,--gc-sections -o $@ $(OUT)/host/lib/*.o $(OUT)/host/test_boss3.o -lm

$(OUT)/test_boss4: $(OUT)/host/lib.stamp test/test_boss4.c
	gcc $(CFLAGS_HOST) -c test/test_boss4.c -o $(OUT)/host/test_boss4.o
	gcc -Wl,--gc-sections -o $@ $(OUT)/host/lib/*.o $(OUT)/host/test_boss4.o -lm

host-test1: $(OUT)/test_boss
	$(OUT)/test_boss $(Z13) $(PLAYER_TABLES) $(OBJDATA) $(SPRITES) $(MODELS) $(OUT)

host-test2: $(OUT)/test_boss2
	$(OUT)/test_boss2 $(Z23) $(PLAYER_TABLES) $(MODELS) $(OUT)

host-test3: $(OUT)/test_boss3
	$(OUT)/test_boss3 $(Z33) $(PLAYER_TABLES) $(MODELS) $(OUT)

host-test4: $(OUT)/test_boss4
	$(OUT)/test_boss4 $(Z43) $(PLAYER_TABLES) $(MODELS) $(OUT)

host-test: host-test1 host-test2 host-test3 host-test4

dc: $(SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

clean:
	rm -rf $(OUT)/test_boss $(OUT)/test_boss2 $(OUT)/test_boss3 $(OUT)/test_boss4 $(OUT)/host $(OUT)/dc

.PHONY: all data host-test host-test1 host-test2 host-test3 host-test4 dc clean
