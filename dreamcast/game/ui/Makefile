# Native UI module (Dreamcast port): HUD, title card, pause, act results, game over.
#   make data       BYOR: build/dreamcast/ui/uidata.bin (ROM tables), navidata.bin + res/ (BBG
#                   backgrounds, navigator, tools/uibg.py) from extract/eur
#   make host-test  build + run the host test (needs data + build/dreamcast/sprites + player tables)
#   make dc         compile all sources with kos-cc (-Werror) into $(OUT)/dc/
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/ui
SPRITES ?= $(ROOT)/build/dreamcast/sprites/bac
PLAYER_TABLES ?= $(ROOT)/build/dreamcast/player
PLAYER_DIR = ../player
OBJ_DIR = ../objects
PLAYER_SRCS = $(PLAYER_DIR)/dcp_math.c $(PLAYER_DIR)/dcp_probe.c $(PLAYER_DIR)/dcp_object.c \
              $(PLAYER_DIR)/dcp_player.c $(PLAYER_DIR)/dcp_states.c
OBJ_SRCS = $(filter-out $(OBJ_DIR)/obj_draw.c,$(wildcard $(OBJ_DIR)/obj_*.c))
SRCS = ui_data.c ui_state.c ui_hud.c ui_titlecard.c ui_pause.c ui_results.c ui_gameover.c ui_draw.c \
       ui_navi.c ui_enemyroom.c ui_bg.c ui_tahud.c
HDRS = ui.h ui_internal.h
INC = -I$(ROOT)/tools/dreamcast/stages
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv $(INC)

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/ui/tools/uidata.py --out $(abspath $(OUT))
	cd $(ROOT) && python3 dreamcast/game/ui/tools/uibg.py --out $(abspath $(OUT))
	cd $(ROOT) && python3 tools/dreamcast/sprites/atlas_pal4.py $(abspath $(OUT))/res

# the player and object modules belong to other workers: built without -Werror here
$(OUT)/test_ui: $(SRCS) $(HDRS) ../save/save.c test/test_ui.c $(PLAYER_SRCS) $(OBJ_SRCS)
	mkdir -p $(OUT)/host
	for f in $(PLAYER_SRCS) $(OBJ_SRCS); do gcc -std=gnu99 -O2 -w -fwrapv $(INC) -c $$f -o $(OUT)/host/$$(basename $${f%.c}).o || exit 1; done
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) ../save/save.c test/test_ui.c $(OUT)/host/*.o -lm

host-test: $(OUT)/test_ui
	$(OUT)/test_ui $(OUT) $(SPRITES) $(PLAYER_TABLES)

dc: $(SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror $(INC) -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

clean:
	rm -rf $(OUT)/test_ui $(OUT)/dc $(OUT)/host

.PHONY: all data host-test dc clean
