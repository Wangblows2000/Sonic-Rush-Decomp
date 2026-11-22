# Stage camera + zone backgrounds / effects (Dreamcast port). Targets:
#   make data        BYOR: build/dreamcast/zonefx/zonefx.bin (needs extract/eur)
#   make host-test   build + run test/test_camera on stages of every zone
#   make dc          compile camera.c and ../zonefx/zonefx.c with kos-cc into $(OUT)/dc/
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/camera
ZFX ?= $(ROOT)/build/dreamcast/zonefx/zonefx.bin
OBJ_DIR = ../objects
PLAYER_DIR = ../player
PLAYER_SRCS = $(PLAYER_DIR)/dcp_math.c $(PLAYER_DIR)/dcp_probe.c $(PLAYER_DIR)/dcp_object.c \
              $(PLAYER_DIR)/dcp_player.c $(PLAYER_DIR)/dcp_states.c
OBJ_SRCS = $(addprefix $(OBJ_DIR)/,obj_anim.c obj_rect.c obj_colmap.c obj_manager.c obj_player.c obj_ring.c \
       obj_gimmicks.c obj_enemy.c obj_fx.c obj_effects2.c obj_leafstorm.c obj_shared.c obj_flapper.c obj_decor.c \
       obj_pgimmick.c obj_water.c obj_pwater.c obj_mirage.c obj_pmirage.c obj_carnival.c obj_pcarnival.c \
       obj_crisis.c obj_pcrisis.c obj_altitude.c obj_paltitude.c obj_deadline.c obj_pdeadline.c obj_enemy2.c \
       obj_palanim.c obj_peffect.c)
SRCS = camera.c ../zonefx/zonefx.c
HDRS = camera.h ../zonefx/zonefx.h
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv
STAGES_DIR ?= $(ROOT)/build/dreamcast/stages
TEST_STAGES ?= z11 z12 z21 z22 z31 z32 z41 z42 z51 z52 z61 z62 z71 z72 z81 z82 z91 z92

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/zonefx/tools/zonefxdata.py

$(OUT)/test_camera: $(SRCS) $(HDRS) test/test_camera.c $(OBJ_SRCS) $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(OBJ_SRCS) $(PLAYER_SRCS) test/test_camera.c -lm

host-test: $(OUT)/test_camera
	$(OUT)/test_camera $(STAGES_DIR) $(ROOT)/build/dreamcast/player $(ROOT)/build/dreamcast/objects \
	    $(ROOT)/build/dreamcast/sprites/bac $(ZFX) $(TEST_STAGES)

dc:
	mkdir -p $(OUT)/dc
	. /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do \
	  kos-cc -O2 -fwrapv -Wall -Wextra -I$(ROOT)/tools/dreamcast/stages -c $$f -o $(OUT)/dc/$$(basename $$f .c).o || exit 1; done
.PHONY: all data host-test dc
