# Native stage object module (Dreamcast port). Targets:
#   make data       BYOR: build/dreamcast/objects/{objdata.bin,hbx/,sprites.txt} (needs extract/eur + sprites)
#   make host-test  build + run the host test on the real z11 level
#   make dc         compile all sources with the KOS toolchain (kos-cc) into $(OUT)/dc/
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/objects
DATA ?= $(OUT)
SPRITES ?= $(ROOT)/build/dreamcast/sprites/bac
PLAYER_DIR = ../player
PLAYER_SRCS = $(PLAYER_DIR)/dcp_math.c $(PLAYER_DIR)/dcp_probe.c $(PLAYER_DIR)/dcp_object.c \
              $(PLAYER_DIR)/dcp_player.c $(PLAYER_DIR)/dcp_states.c
SRCS = obj_anim.c obj_rect.c obj_colmap.c obj_manager.c obj_player.c obj_ring.c obj_gimmicks.c obj_enemy.c
DC_SRCS = $(SRCS) obj_draw.c
HDRS = obj_types.h obj_internal.h objects.h $(PLAYER_DIR)/dcp_player.h $(ROOT)/tools/dreamcast/stages/level_format.h
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv
Z11 ?= $(ROOT)/build/dreamcast/stages/z11/level.bin
PLAYER_TABLES ?= $(ROOT)/build/dreamcast/player

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/objects/tools/objdata.py --out $(abspath $(DATA))

$(OUT)/test_objects: $(SRCS) $(HDRS) test/test_objects.c $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(PLAYER_SRCS) test/test_objects.c -lm

host-test: $(OUT)/test_objects
	$(OUT)/test_objects $(Z11) $(PLAYER_TABLES) $(DATA) $(SPRITES)

dc: $(DC_SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(DC_SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

clean:
	rm -rf $(OUT)/test_objects $(OUT)/dc

.PHONY: all data host-test dc clean
