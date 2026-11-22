# Native save data (VMU). See docs/dreamcast/translation/MENUS.md section 5.
#   make host-test  serialisation / slot rotation / recovery test (host gcc)
#   make dc         kos-cc -Werror compile check
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/menus
MENUDATA ?= $(OUT)/menudata.bin

all: host-test

$(OUT)/test_save: save.c save.h test/test_save.c
	mkdir -p $(OUT)
	gcc -std=c99 -O2 -Wall -Wextra -Werror -fwrapv -o $@ save.c test/test_save.c

host-test: $(OUT)/test_save
	mkdir -p /tmp/menus
	$(OUT)/test_save $(MENUDATA) /tmp/menus

dc:
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in save.c save_vmu.c; do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done'

.PHONY: all host-test dc
