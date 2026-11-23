# Battle Play rules (vs.c). make host-test: test/test_vs on Leaf Storm act 1 with the real player and objects.
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/vs
OBJ_DIR = ../objects
PLAYER_DIR = ../player
PLAYER_SRCS = $(PLAYER_DIR)/dcp_math.c $(PLAYER_DIR)/dcp_probe.c $(PLAYER_DIR)/dcp_object.c \
              $(PLAYER_DIR)/dcp_player.c $(PLAYER_DIR)/dcp_states.c
OBJ_SRCS = $(filter-out $(OBJ_DIR)/obj_draw.c,$(wildcard $(OBJ_DIR)/obj_*.c))
SRCS = vs.c ../camera/camera.c ../zonefx/zonefx.c
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv
all: host-test
$(OUT)/test_vs: $(SRCS) vs.h test/test_vs.c $(OBJ_SRCS) $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(OBJ_SRCS) $(PLAYER_SRCS) test/test_vs.c -lm
host-test: $(OUT)/test_vs
	$(OUT)/test_vs $(ROOT)/build/dreamcast/stages $(ROOT)/build/dreamcast/player $(ROOT)/build/dreamcast/objects $(ROOT)/build/dreamcast/sprites/bac
.PHONY: all host-test
