# Attract demos (Dreamcast port): DemoInput / DemoPlay. See docs/dreamcast/translation/MENUS.md section 10.
#   make data       BYOR: build/dreamcast/demo/ (demotab.bin + key/keyNN.bin)
#   make host-test  build + run the host test on that data
#   make dc         compile with kos-cc -Werror into $(OUT)/dc/
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/demo
SRCS = demo.c
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/demo/tools/demodata.py --out $(abspath $(OUT))

$(OUT)/host/test_demo: $(SRCS) demo.h test/test_demo.c
	mkdir -p $(OUT)/host
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) test/test_demo.c

host-test: $(OUT)/host/test_demo
	$(OUT)/host/test_demo $(abspath $(OUT))

dc: $(SRCS) demo.h
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

clean:
	rm -rf $(OUT)/host $(OUT)/dc

.PHONY: all data host-test dc clean
