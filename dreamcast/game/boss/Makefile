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
SRCS = boss_core.c boss_camera.c boss_model.c boss_effect.c boss1.c boss2.c boss3.c boss4.c boss5.c
SRCS += boss6.c
SRCS += boss7.c
SRCS += boss_final.c
SRCS += boss_extra.c boss_extra_obj.c boss_extra_player.c boss_extra_weapon.c boss_extra_hand.c \
        boss_extra_stage.c boss_extra_draw.c
HDRS = boss.h boss_internal.h
HDRS += boss_extra.h boss_extra_int.h
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
Z53 ?= $(ROOT)/build/dreamcast/stages/z53/level.bin

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

host-test5: $(OUT)/test_boss
	$(OUT)/test_boss $(Z53) $(PLAYER_TABLES) $(OBJDATA) $(SPRITES) $(MODELS) $(OUT)

# Boss6 (z63, Altitude Limit)
Z63 ?= $(ROOT)/build/dreamcast/stages/z63/level.bin
$(OUT)/test_boss6: $(OUT)/host/lib.stamp test/test_boss6.c
	gcc $(CFLAGS_HOST) -c test/test_boss6.c -o $(OUT)/host/test_boss6.o
	gcc -Wl,--gc-sections -o $@ $(OUT)/host/lib/*.o $(OUT)/host/test_boss6.o -lm

host-test6: $(OUT)/test_boss6
	$(OUT)/test_boss6 $(Z63) $(PLAYER_TABLES) $(MODELS) $(OUT)
host-test: host-test6
clean: clean-boss6
clean-boss6:
	rm -f $(OUT)/test_boss6
.PHONY: host-test6 clean-boss6

# Boss7 (z73, Dead Line): the rival fight + clash.  STORY=1: the story character is Blaze (rival Sonic)
Z73 ?= $(ROOT)/build/dreamcast/stages/z73/level.bin
$(OUT)/test_boss7: $(OUT)/host/lib.stamp test/test_boss7.c boss7.h
	gcc $(CFLAGS_HOST) -c test/test_boss7.c -o $(OUT)/host/test_boss7.o
	gcc -Wl,--gc-sections -o $@ $(OUT)/host/lib/*.o $(OUT)/host/test_boss7.o -lm

host-test7: $(OUT)/test_boss7
	$(OUT)/test_boss7 $(Z73) $(PLAYER_TABLES) $(MODELS) $(OUT)
host-test: host-test7
clean: clean-boss7
clean-boss7:
	rm -f $(OUT)/test_boss7
.PHONY: host-test7 clean-boss7

# BossF (zf3, the final zone boss): STORY=1 plays Blaze
ZF3 ?= $(ROOT)/build/dreamcast/stages/zf3/level.bin
$(OUT)/test_bossf: $(OUT)/host/lib.stamp test/test_bossf.c boss_final.h
	gcc $(CFLAGS_HOST) -c test/test_bossf.c -o $(OUT)/host/test_bossf.o
	gcc -Wl,--gc-sections -o $@ $(OUT)/host/lib/*.o $(OUT)/host/test_bossf.o -lm

host-testF: $(OUT)/test_bossf
	$(OUT)/test_bossf $(ZF3) $(PLAYER_TABLES) $(MODELS) $(OUT)
	STORY=1 LEVEL=1 SEED=77 $(OUT)/test_bossf $(ZF3) $(PLAYER_TABLES) $(MODELS) $(OUT)
	NOCROUCH=1 $(OUT)/test_bossf $(ZF3) $(PLAYER_TABLES) $(MODELS) $(OUT)
host-test: host-testF
clean: clean-bossf
clean-bossf:
	rm -f $(OUT)/test_bossf
.PHONY: host-testF clean-bossf

# Extra Zone boss (stage 26, boss_extra*.c): self-contained mode, no level.bin.
#   make data-ex      BYOR: build/dreamcast/boss/extra/ (bossex.bin, zex.bin, vs_*.bin, sprites/, assets.txt)
#   make host-testEX  the whole five-round fight (scripted pad), seeds, game over, pause; frames in /tmp/bossex
EXTRA_DATA ?= $(OUT)/extra
data-ex:
	cd $(ROOT) && python3 dreamcast/game/boss/tools/bossexdata.py --out $(abspath $(EXTRA_DATA))
$(OUT)/test_bossex: $(OUT)/host/lib.stamp test/test_bossex.c
	gcc $(CFLAGS_HOST) -c test/test_bossex.c -o $(OUT)/host/test_bossex.o
	gcc -Wl,--gc-sections -o $@ $(OUT)/host/lib/*.o $(OUT)/host/test_bossex.o -lm
host-testEX: $(OUT)/test_bossex
	$(OUT)/test_bossex $(PLAYER_TABLES) $(MODELS) $(EXTRA_DATA)
host-test: host-testEX
clean: clean-bossex
clean-bossex:
	rm -f $(OUT)/test_bossex
.PHONY: data-ex host-testEX clean-bossex

host-test: host-test1 host-test2 host-test3 host-test4 host-test5

dc: $(SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

clean:
	rm -rf $(OUT)/test_boss $(OUT)/test_boss2 $(OUT)/test_boss3 $(OUT)/test_boss4 $(OUT)/host $(OUT)/dc

.PHONY: all data host-test host-test1 host-test2 host-test3 host-test4 host-test5 dc clean
