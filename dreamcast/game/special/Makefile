# Native special-stage module (Dreamcast port). Targets:
#   make data       BYOR: build/dreamcast/special/ (tools/spdata.py; needs extract/eur and the
#                   audio pipeline output build/dreamcast/audio for the sound bank)
#   make host-test  build + run the host tests on the converted data (all 7 stages)
#   make dc         compile every source with kos-cc -Wall -Wextra -Werror into $(OUT)/dc/
#   make test-elf   standalone test program (KOS ELF, special stage only) -> $(OUT)/dc/sptest.elf
#   make test-disc  test-elf + a CDI with the converted assets -> $(OUT)/disc/sptest.cdi
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/special
SRCS = sp_math.c sp_data.c sp_sprite.c sp_model.c sp_map.c sp_gimmick.c sp_player.c sp_camera.c sp_hud.c \
       sp_core.c sp_input.c sp_draw.c sp_audio.c
HDRS = special.h sp_internal.h
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv -g
MODEL = ../model/model.c

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/special/tools/spdata.py

$(OUT)/host/test_special: $(SRCS) $(HDRS) $(MODEL) test/test_special.c
	mkdir -p $(OUT)/host
	gcc $(CFLAGS_HOST) -I. $(SRCS) $(MODEL) test/test_special.c -o $@ -lm

host-test: $(OUT)/host/test_special
	$(OUT)/host/test_special $(abspath $(OUT))

dc: $(SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

test-elf: dc test/sptest_main.c
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c test/sptest_main.c -o $(OUT)/dc/sptest_main.o && \
	  kos-cc -std=gnu99 -O2 -fwrapv -c ../model/model.c -o $(OUT)/dc/model.o && \
	  for f in ../engine/render.c ../engine/input.c ../engine/audio.c ../sound/snd_bank.c ../sound/snd_drv.c ../sound/snd_nns.c ../sound/sound.c ../sound/snd_out_kos.c; do \
	    kos-cc -std=gnu99 -O2 -fwrapv -I.. -c $$f -o $(OUT)/dc/x_$$(basename $${f%.c}).o || exit 1; done && \
	  kos-cc -o $(OUT)/dc/sptest.elf $(OUT)/dc/sptest_main.o $(patsubst %.c,$(OUT)/dc/%.o,$(SRCS)) $(OUT)/dc/model.o $(OUT)/dc/x_*.o -lm'

test-disc: test-elf
	bash test/make_test_disc.sh $(abspath $(ROOT)) $(abspath $(OUT))

clean:
	rm -rf $(OUT)/host $(OUT)/dc $(OUT)/disc

.PHONY: all data host-test dc test-elf test-disc clean
