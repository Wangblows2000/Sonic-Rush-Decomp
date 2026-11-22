# Native boss module (Dreamcast port). Targets:
#   make data       BYOR: build/dreamcast/boss/{bossdata.bin,assets.txt} (needs extract/eur, models converted)
#   make host-test  build + run the host test (Egg Hammer on the real z13 level)
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
SRCS = boss_core.c boss_camera.c boss_model.c boss_effect.c boss1.c
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

$(OUT)/test_boss: $(SRCS) $(HDRS) test/test_boss.c $(PLAYER_SRCS) $(OBJ_SRCS) $(MODEL_DIR)/model.c
	rm -rf $(OUT)/host && mkdir -p $(OUT)/host
	for f in $(SRCS) test/test_boss.c; do gcc $(CFLAGS_HOST) -c $$f -o $(OUT)/host/b_$$(basename $${f%.c}).o || exit 1; done
	for f in $(PLAYER_SRCS) $(OBJ_SRCS) $(MODEL_DIR)/model.c test/stubs_objects.c; do gcc -std=c99 -O2 -fwrapv -c $$f -o $(OUT)/host/x_$$(basename $${f%.c}).o || exit 1; done
	gcc -o $@ $(OUT)/host/*.o -lm

host-test: $(OUT)/test_boss
	$(OUT)/test_boss $(Z13) $(PLAYER_TABLES) $(OBJDATA) $(SPRITES) $(MODELS) $(OUT)

dc: $(SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

clean:
	rm -rf $(OUT)/test_boss $(OUT)/dc

.PHONY: all data host-test dc clean
