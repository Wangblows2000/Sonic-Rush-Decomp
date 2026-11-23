# Native stage object module (Dreamcast port). Targets:
#   make data       BYOR: build/dreamcast/objects/{objdata.bin,hbx/,sprites.txt} (needs extract/eur + sprites)
#   make host-test  build + run the host tests: test_objects on z11/z12, test_zones on $(ZONE_STAGES), audit
#   make audit      the item-C gimmick audit (test_objects AUDIT_ONLY=1) on $(AUDIT_STAGES), Sonic and Blaze
#   make coverage   create every placed object/decoration of every stage, print the coverage table
#   make preload    $(OUT)/preload.txt: the sprite resources objects_init loads per stage (disc stage packs)
#   make dc         compile all sources with the KOS toolchain (kos-cc) into $(OUT)/dc/
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/objects
DATA ?= $(OUT)
SPRITES ?= $(ROOT)/build/dreamcast/sprites/bac
PLAYER_DIR = ../player
PLAYER_SRCS = $(PLAYER_DIR)/dcp_math.c $(PLAYER_DIR)/dcp_probe.c $(PLAYER_DIR)/dcp_object.c \
              $(PLAYER_DIR)/dcp_player.c $(PLAYER_DIR)/dcp_states.c
SRCS = obj_anim.c obj_rect.c obj_colmap.c obj_manager.c obj_player.c obj_ring.c obj_gimmicks.c obj_enemy.c \
       obj_fx.c obj_effects2.c obj_leafstorm.c obj_shared.c obj_flapper.c obj_decor.c obj_pgimmick.c \
       obj_water.c obj_pwater.c obj_mirage.c obj_pmirage.c \
       obj_carnival.c obj_pcarnival.c obj_crisis.c obj_pcrisis.c obj_altitude.c obj_paltitude.c obj_deadline.c obj_pdeadline.c obj_enemy2.c obj_palanim.c obj_peffect.c obj_vs.c
DC_SRCS = $(SRCS) obj_draw.c
HDRS = obj_types.h obj_internal.h obj_zone.h objects.h $(PLAYER_DIR)/dcp_player.h $(ROOT)/tools/dreamcast/stages/level_format.h
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv
Z11 ?= $(ROOT)/build/dreamcast/stages/z11/level.bin
Z12 ?= $(ROOT)/build/dreamcast/stages/z12/level.bin
PLAYER_TABLES ?= $(ROOT)/build/dreamcast/player

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/objects/tools/objdata.py --out $(abspath $(DATA))

$(OUT)/test_objects: $(SRCS) $(HDRS) test/test_objects.c $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(PLAYER_SRCS) test/test_objects.c -lm

$(OUT)/test_zones: $(SRCS) $(HDRS) test/test_zones.c $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(PLAYER_SRCS) test/test_zones.c -lm

STAGES_DIR ?= $(ROOT)/build/dreamcast/stages
ZONE_STAGES = z21 z22 z31 z32 z41 z42 z51 z52 z61 z62 z71 z72 z91 z92
host-test: $(OUT)/test_objects $(OUT)/test_zones
	$(OUT)/test_objects $(Z11) $(PLAYER_TABLES) $(DATA) $(SPRITES)
	$(OUT)/test_objects $(Z12) $(PLAYER_TABLES) $(DATA) $(SPRITES)
	set -e; for s in $(ZONE_STAGES); do $(OUT)/test_zones $(STAGES_DIR)/$$s/level.bin $(PLAYER_TABLES) $(DATA) $(SPRITES); done
	$(MAKE) --no-print-directory audit

# the per-class gimmick audit (springs, dash panels, needles, item boxes, markers, ...) on every
# stage, for Sonic and Blaze
AUDIT_STAGES = z11 z12 $(ZONE_STAGES)
audit: $(OUT)/test_objects
	set -e; for c in 0 1; do for s in $(AUDIT_STAGES); do echo "== $$s chara $$c"; \
	  CHARA=$$c AUDIT_ONLY=1 $(OUT)/test_objects $(STAGES_DIR)/$$s/level.bin $(PLAYER_TABLES) $(DATA) $(SPRITES) \
	    > $(OUT)/audit.log || { grep -v '^    ' $(OUT)/audit.log; exit 1; }; \
	  grep -v '^    \|^objects_init\|slots preloaded' $(OUT)/audit.log; done; done; rm -f $(OUT)/audit.log

zone-test: $(OUT)/test_zones
	set -e; for s in $(ZONE_STAGES); do $(OUT)/test_zones $(STAGES_DIR)/$$s/level.bin $(PLAYER_TABLES) $(DATA) $(SPRITES); done

STAGES = z11 z12 z13 z21 z22 z23 z31 z32 z33 z41 z42 z43 z51 z52 z53 z61 z62 z63 z71 z72 z73 z81 z82 z91 z92 zf3
$(OUT)/test_coverage: $(SRCS) $(HDRS) test/test_coverage.c $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(PLAYER_SRCS) test/test_coverage.c -lm

# the resources objects_init preloads per stage (tools/dreamcast/mkpak.py --stage-packs)
preload: $(OUT)/test_coverage
	COV_RES=1 $(OUT)/test_coverage $(PLAYER_TABLES) $(DATA) $(SPRITES) $(ROOT)/build/dreamcast/stages $(STAGES) | grep '^res ' > $(OUT)/preload.txt

coverage: $(OUT)/test_coverage
	$(OUT)/test_coverage $(PLAYER_TABLES) $(DATA) $(SPRITES) $(ROOT)/build/dreamcast/stages $(STAGES)

dc: $(DC_SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(DC_SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

clean:
	rm -rf $(OUT)/test_objects $(OUT)/test_zones $(OUT)/test_coverage $(OUT)/dc

.PHONY: all data host-test audit zone-test coverage preload dc clean
