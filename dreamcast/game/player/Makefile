# Native player module (Dreamcast port). Targets:
#   make host-test   build + run the host unit test (needs build/dreamcast/player from tools/extract_physics.py)
#   make dc          compile all sources with the KOS toolchain (kos-cc) into build/dreamcast/player/dc/
#   make tables      run the BYOR table converter
#   make model-test  host test of the player model presentation (dcp_model.c) on the converted models
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/player
SRCS = dcp_math.c dcp_probe.c dcp_object.c dcp_player.c dcp_states.c
HDRS = dcp_types.h dcp_collision.h dcp_player.h $(ROOT)/tools/dreamcast/stages/level_format.h
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv
Z11 ?= $(ROOT)/build/dreamcast/stages/z11/level.bin

all: host-test

tables:
	cd $(ROOT) && python3 -I dreamcast/game/player/tools/extract_physics.py

$(OUT)/test_player: $(SRCS) $(HDRS) test/test_player.c
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) test/test_player.c -lm

host-test: $(OUT)/test_player
	$(OUT)/test_player $(OUT) $(wildcard $(Z11))

dc: $(SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do kos-cc -std=c99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

# player model presentation (dcp_model.c + ../model/model.c) on the converted assets
MODELS ?= $(ROOT)/build/dreamcast/models
$(OUT)/test_model: $(SRCS) dcp_model.c dcp_model.h $(HDRS) ../model/model.c test/test_model.c
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) dcp_model.c ../model/model.c test/test_model.c -lm

model-test: $(OUT)/test_model
	mkdir -p $(OUT)/model
	cp $(MODELS)/mod__son__son.dcm $(OUT)/model/son.dcm; cp $(MODELS)/mod__son__son_ball.dcm $(OUT)/model/son_ball.dcm
	cp $(MODELS)/mod__blz__blz.dcm $(OUT)/model/blz.dcm; cp $(MODELS)/mod__blz__blz_spin.dcm $(OUT)/model/blz_spin.dcm
	cp $(MODELS)/narc__player_lz7__son.dca $(OUT)/model/son.dca; cp $(MODELS)/narc__player_lz7__son.dcv $(OUT)/model/son.dcv
	cp $(OUT)/animtab.bin $(OUT)/lights.bin $(OUT)/model/
	$(OUT)/test_model $(OUT)/model $(OUT) $(wildcard $(Z11))

clean:
	rm -rf $(OUT)/test_player $(OUT)/test_model $(OUT)/model $(OUT)/dc

.PHONY: all tables host-test model-test dc clean
