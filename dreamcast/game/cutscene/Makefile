# Native cutscenes (Dreamcast port): TalkDemo, PlayerDemo/PolyDemo, TransformDemo,
# DemoScript, Ending/ExtraEnding/StaffRoll/PostStaffRoll. docs/dreamcast/translation/CUTSCENES.md
#   make data       BYOR: build/dreamcast/cutscene/ (csdata.bin, talk/, pldm/, bg/, sound/ sys_aNN.srb,
#                   sprites.txt, models.txt, music.txt); needs the sprite, model and audio converters' output
#   make host-test  build + run the host test (runs every cutscene headless; DUMP=dir writes PPM frames)
#   make dc         compile all sources with kos-cc -Wall -Wextra -Werror into $(OUT)/dc/
#   make link       KOS link test (cutscene + model + engine render/input/audio + sound + a test main)
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/cutscene
SPRITES ?= $(ROOT)/build/dreamcast/sprites/bac
MODELS ?= $(ROOT)/build/dreamcast/models
DUMP ?=
SRCS = cs_core.c cs_draw.c cs_3d.c cs_sound.c cs_main.c cs_talk.c cs_script.c cs_player.c cs_trans.c cs_ending.c
HDRS = cutscene.h cs_internal.h ../model/model.h ../model/model_format.h
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv -DCS_HOST

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/cutscene/tools/csdata.py --out $(abspath $(OUT))
	cd $(ROOT) && python3 dreamcast/game/cutscene/tools/csbank.py --out $(abspath $(OUT))/sound

$(OUT)/test_cutscene: $(SRCS) $(HDRS) test/test_cutscene.c test/cs_raster.h ../model/model.c
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) ../model/model.c test/test_cutscene.c -lm

host-test: $(OUT)/test_cutscene
	$(if $(DUMP),mkdir -p $(DUMP))
	$(OUT)/test_cutscene $(OUT) $(SPRITES) $(MODELS) $(DUMP)

dc: $(SRCS) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$${f%.c}.o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

link: dc test/link_main.c
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && kos-cc -std=gnu99 -O2 -Wall -Werror -c test/link_main.c -o $(OUT)/dc/link_main.o && kos-cc -std=gnu99 -O2 -fwrapv -c ../model/model.c -o $(OUT)/dc/model.o && for f in ../engine/render.c ../engine/input.c ../engine/audio.c ../engine/pak.c ../engine/loading.c ../sound/sound.c ../sound/snd_bank.c ../sound/snd_drv.c ../sound/snd_nns.c ../sound/snd_out_kos.c; do kos-cc -std=gnu99 -O2 -fwrapv -c $$f -o $(OUT)/dc/x_$$(basename $${f%.c}).o || exit 1; done && kos-cc -o $(OUT)/cutscene_test.elf $(OUT)/dc/*.o -lm && sh-elf-size $(OUT)/cutscene_test.elf'

clean:
	rm -rf $(OUT)/test_cutscene $(OUT)/dc $(OUT)/cutscene_test.elf

.PHONY: all data host-test dc link clean
