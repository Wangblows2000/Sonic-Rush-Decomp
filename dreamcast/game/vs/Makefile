# Battle Play rules (vs.c). make host-test: test/test_vs on Leaf Storm act 1 with the real player and objects.
ROOT ?= ../../..
OUT ?= $(ROOT)/build/dreamcast/vs
OBJ_DIR = ../objects
PLAYER_DIR = ../player
PLAYER_SRCS = $(PLAYER_DIR)/dcp_math.c $(PLAYER_DIR)/dcp_probe.c $(PLAYER_DIR)/dcp_object.c \
              $(PLAYER_DIR)/dcp_player.c $(PLAYER_DIR)/dcp_states.c
# the players' models in the simulation (animation ends drive player states; no drawing)
MODEL_SRCS = $(PLAYER_DIR)/dcp_model.c ../model/model.c
OBJ_SRCS = $(filter-out $(OBJ_DIR)/obj_draw.c,$(wildcard $(OBJ_DIR)/obj_*.c))
SRCS = vs.c ../camera/camera.c ../zonefx/zonefx.c
CFLAGS_HOST = -std=c99 -O2 -Wall -Wextra -Werror -fwrapv
all: host-test
$(OUT)/test_vs: $(SRCS) vs.h test/test_vs.c $(OBJ_SRCS) $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) $(OBJ_SRCS) $(PLAYER_SRCS) test/test_vs.c -lm
$(OUT)/test_cpu: $(SRCS) vs.h vs_cpu.c vs_cpu.h vs_route.h test/test_cpu.c route/vs_sim.c route/vs_sim.h $(OBJ_SRCS) $(PLAYER_SRCS) $(MODEL_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) vs_cpu.c $(OBJ_SRCS) $(PLAYER_SRCS) $(MODEL_SRCS) route/vs_sim.c test/test_cpu.c -lm
$(OUT)/test_cpu_unit: vs_cpu.c vs_cpu.h vs_route.h test/test_cpu_unit.c $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ vs_cpu.c $(PLAYER_SRCS) test/test_cpu_unit.c -lm
host-test: $(OUT)/test_vs $(OUT)/test_cpu_unit
	$(OUT)/test_vs $(ROOT)/build/dreamcast/stages $(ROOT)/build/dreamcast/player $(ROOT)/build/dreamcast/objects $(ROOT)/build/dreamcast/sprites/bac
	$(OUT)/test_cpu_unit
CPU_STAGES = z11 z12 z21 z22 z31 z32 z41 z42 z51 z52 z61 z62 z71 z72
cpu-test: $(OUT)/test_cpu
	@cd $(ROOT)/build/dreamcast && for s in $(CPU_STAGES); do for c in 0 1; do ./vs/test_cpu stages player objects sprites/bac $$s $$c vs/routes/$${s}_$$c.rte $(CPU_ARGS); done; done
# the sources a racing line depends on (the simulation, the follower, the tool): their hash goes into
# every line's generation key, so changing any of them makes the lines again (and nothing else does)
VSR_DEPS = $(sort $(SRCS) $(OBJ_SRCS) $(PLAYER_SRCS) $(MODEL_SRCS) ../model/*.h vs_cpu.c route/vs_sim.c route/snap.c route/vs_route.c \
           $(wildcard *.h route/*.h $(OBJ_DIR)/*.h $(PLAYER_DIR)/*.h ../camera/*.h ../zonefx/*.h))
VSR_SRC := $(shell cat $(VSR_DEPS) | sha1sum | cut -c1-16)
vsr-src:
	@echo $(VSR_SRC)
ROUTE_SIM = route/snap_begin.c $(SRCS) $(OBJ_SRCS) $(PLAYER_SRCS) $(MODEL_SRCS) route/vs_sim.c route/snap_end.c
$(OUT)/vs_route: $(SRCS) vs.h vs_cpu.h vs_cpu.c vs_route.h route/*.c route/*.h $(OBJ_SRCS) $(PLAYER_SRCS) $(MODEL_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -fno-common -DVSR_SRC='"$(VSR_SRC)"' -o $@ $(ROUTE_SIM) route/snap.c vs_cpu.c route/vs_route.c -lm
# Vs CPU racing lines (disc build: tools/dreamcast/disc/assemble_disc.py -> /cd/vs): one per stage and
# character, made from the converted stages with the real simulation (route/vs_route.c). Every pair runs
# with -check: a line whose generation key (stage, objects, player data, sources, version) is unchanged is
# kept at once, the others are made again; a stopped run carries on with the pairs not yet made. Report
# per pair: routes/<pair>.rte.txt. Exit 3: a partial line (no way to the goal found).
ROUTE_BUDGET ?= 3600
ROUTE_ARGS ?=
ROUTES := $(foreach s,$(CPU_STAGES),$(foreach c,0 1,$(s)_$(c)))
# the converted player models where the simulation reads them (build/dreamcast/player/model; the disc's
# /cd/player names)
MODELS = $(ROOT)/build/dreamcast/models
PM = $(ROOT)/build/dreamcast/player
simmodel:
	@mkdir -p $(PM)/model
	@cp $(MODELS)/mod__son__son.dcm $(PM)/model/son.dcm; cp $(MODELS)/mod__son__son_ball.dcm $(PM)/model/son_ball.dcm
	@cp $(MODELS)/mod__blz__blz.dcm $(PM)/model/blz.dcm; cp $(MODELS)/mod__blz__blz_spin.dcm $(PM)/model/blz_spin.dcm
	@cp $(MODELS)/narc__player_lz7__son.dca $(PM)/model/son.dca; cp $(MODELS)/narc__player_lz7__son.dcv $(PM)/model/son.dcv
	@cp $(PM)/animtab.bin $(PM)/lights.bin $(PM)/model/
cpu-test: simmodel
routes: $(addprefix route-,$(ROUTES))
	@cd $(ROOT)/build/dreamcast/vs/routes && cat $(addsuffix .rte.txt,$(ROUTES)) 2>/dev/null | grep "chara" || true
route-%: $(OUT)/vs_route | simmodel
	@mkdir -p $(OUT)/routes
	@cd $(ROOT)/build/dreamcast && { ./vs/vs_route stages player objects sprites/bac $(word 1,$(subst _, ,$*)) $(word 2,$(subst _, ,$*)) \
	    vs/routes/$*.rte -check -budget $(ROUTE_BUDGET) $(ROUTE_ARGS) > vs/routes/$*.log 2>&1; tail -1 vs/routes/$*.log; }
.PHONY: all host-test cpu-test routes vsr-src simmodel
