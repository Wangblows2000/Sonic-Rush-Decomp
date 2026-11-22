# Native front end (Dreamcast port): SegaLogo, TitleScreen, TitleMenu, PlayerSelect,
# ZoneMap (+ act select, map pause), Options. See docs/dreamcast/translation/MENUS.md.
#   make data       BYOR: build/dreamcast/menus/ (menudata.bin, bg/*.mnbg, ama/, sound_test.sld)
#   make host-test  build + run the host test (needs data + build/dreamcast/sprites/bac)
#   make dc         compile all sources (+ ../save) with kos-cc -Werror into $(OUT)/dc/
#   make link       KOS link test: menus + save + engine/render.c + input.c + a stub main
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/menus
SPRITES ?= $(ROOT)/build/dreamcast/sprites/bac
DUMP ?=
SRCS = mn_core.c mn_draw.c mn_main.c mn_story.c mn_title.c mn_titlemenu.c mn_zonemap.c mn_actsel.c \
       mn_psel.c mn_options.c mn_timeattack.c mn_tryagain.c mn_records.c mn_extraact.c mn_navi.c
SAVE = ../save/save.c ../save/save_vmu.c
HDRS = menus.h mn_internal.h extraact.h ../save/save.h
MODEL = ../model/model.c
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv -I..

all: host-test

data:
	cd $(ROOT) && python3 dreamcast/game/menus/tools/menudata.py --out $(abspath $(OUT))

$(OUT)/test_menus: $(SRCS) $(HDRS) test/test_menus.c test/mn_raster.h ../save/save.c
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(MODEL) ../save/save.c test/test_menus.c -lm

$(OUT)/test_flow: $(SRCS) $(HDRS) test/test_flow.c test/mn_raster.h ../save/save.c
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(MODEL) ../save/save.c test/test_flow.c -lm

host-test: $(OUT)/test_menus $(OUT)/test_flow
	mkdir -p /tmp/menus
	$(OUT)/test_menus $(OUT) $(SPRITES) $(DUMP)
	$(OUT)/test_flow $(OUT) $(SPRITES) $(DUMP)

# whole-story flow (menus + save + cutscenes, a model of main.c's scene flow); needs the cutscene data
CS_SRCS = $(addprefix ../cutscene/,cs_core.c cs_draw.c cs_3d.c cs_sound.c cs_main.c cs_talk.c cs_script.c cs_player.c cs_trans.c cs_ending.c)
CSDIR ?= $(ROOT)/build/dreamcast/cutscene
MODELS ?= $(ROOT)/build/dreamcast/models
$(OUT)/test_story: $(SRCS) $(HDRS) test/test_story.c ../save/save.c $(CS_SRCS) ../cutscene/cutscene.h
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -DCS_HOST -o $@ $(SRCS) $(MODEL) ../save/save.c $(CS_SRCS) test/test_story.c -lm
story-test: $(OUT)/test_story
	$(OUT)/test_story $(OUT) $(SPRITES) $(CSDIR) $(MODELS)

dc: $(SRCS) $(SAVE) $(HDRS)
	mkdir -p $(OUT)/dc
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && for f in $(SRCS) $(SAVE); do kos-cc -std=gnu99 -O2 -fwrapv -Wall -Wextra -Werror -c $$f -o $(OUT)/dc/$$(basename $${f%.c}).o || exit 1; done && sh-elf-size $(OUT)/dc/*.o'

link: dc test/link_main.c
	bash -c 'source /opt/toolchains/dc/kos/environ.sh && kos-cc -std=gnu99 -O2 -Wall -Werror -I.. -c test/link_main.c -o $(OUT)/dc/link_main.o && kos-cc -std=gnu99 -O2 -c ../engine/render.c -o $(OUT)/dc/render.o && kos-cc -std=gnu99 -O2 -c ../engine/input.c -o $(OUT)/dc/input.o && kos-cc -std=gnu99 -O2 -fwrapv -c ../model/model.c -o $(OUT)/dc/model.o && kos-cc -o $(OUT)/menus_link.elf $(OUT)/dc/*.o -lm && sh-elf-size $(OUT)/menus_link.elf'

clean:
	rm -rf $(OUT)/test_menus $(OUT)/dc $(OUT)/menus_link.elf

.PHONY: all data host-test story-test dc link clean
