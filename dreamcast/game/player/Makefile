# Native player module (Dreamcast port). Targets:
#   make host-test   build + run the host unit test (needs build/dreamcast/player from tools/extract_physics.py)
#   make dc          compile all sources with the KOS toolchain (kos-cc) into build/dreamcast/player/dc/
#   make tables      run the BYOR table converter
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

clean:
	rm -rf $(OUT)/test_player $(OUT)/dc

.PHONY: all tables host-test dc clean
