# Native sound effects runtime (Dreamcast port). Targets:
#   make bank       BYOR: build/dreamcast/audio/sfxbank/*.srb (needs extract/eur + sdat2dc.py output)
#   make host-test  build + run the host tests (id mapping, players/priorities, render vs sseqrender)
#   make dc         compile all sources with kos-cc -Werror into $(OUT)/dc/
#   make cdi        Flycast test disc: $(OUT)/sndtest.cdi (test/dc_sndtest.c + z1s bank)
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/audio/sfxbank
SRCS = snd_bank.c snd_drv.c snd_nns.c sound.c
DC_SRCS = $(SRCS) sound_hooks.c snd_out_kos.c
HDRS = snd_internal.h snd_nns.h sound.h
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv -D_POSIX_C_SOURCE=200809L

all: host-test

bank:
	cd $(ROOT) && python3 tools/dreamcast/audio/sfxbank.py

$(OUT)/test_sound: $(SRCS) sound_hooks.c $(HDRS) test/test_sound.c test/snd_out_host.c test/snd_out_host.h
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -I. -I$(ROOT)/tools/dreamcast/stages -o $@ $(SRCS) sound_hooks.c test/snd_out_host.c test/test_sound.c -lm

host-test: $(OUT)/test_sound
	$(OUT)/test_sound $(OUT) $(ROOT)/build/dreamcast/audio

dc: $(DC_SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(DC_SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

clean:
	rm -rf $(OUT)/test_sound $(OUT)/dc

.PHONY: all bank host-test dc clean
