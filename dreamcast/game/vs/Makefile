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
$(OUT)/test_cpu: $(SRCS) vs.h vs_cpu.c vs_cpu.h vs_route.h test/test_cpu.c route/vs_sim.c route/vs_sim.h $(OBJ_SRCS) $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ $(SRCS) vs_cpu.c $(OBJ_SRCS) $(PLAYER_SRCS) route/vs_sim.c test/test_cpu.c -lm
$(OUT)/test_cpu_unit: vs_cpu.c vs_cpu.h vs_route.h test/test_cpu_unit.c $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -o $@ vs_cpu.c $(PLAYER_SRCS) test/test_cpu_unit.c -lm
host-test: $(OUT)/test_vs $(OUT)/test_cpu_unit
	$(OUT)/test_vs $(ROOT)/build/dreamcast/stages $(ROOT)/build/dreamcast/player $(ROOT)/build/dreamcast/objects $(ROOT)/build/dreamcast/sprites/bac
	$(OUT)/test_cpu_unit
CPU_STAGES = z11 z12 z21 z22 z31 z32 z41 z42 z51 z52 z61 z62 z71 z72
cpu-test: $(OUT)/test_cpu
	@cd $(ROOT)/build/dreamcast && for s in $(CPU_STAGES); do for c in 0 1; do ./vs/test_cpu stages player objects sprites/bac $$s $$c vs/routes/$${s}_$$c.rte $(CPU_ARGS); done; done
ROUTE_SIM = route/snap_begin.c $(SRCS) $(OBJ_SRCS) $(PLAYER_SRCS) route/vs_sim.c route/snap_end.c
$(OUT)/vs_route: $(SRCS) vs.h vs_cpu.h vs_cpu.c vs_route.h route/*.c route/*.h $(OBJ_SRCS) $(PLAYER_SRCS)
	mkdir -p $(OUT)
	gcc $(CFLAGS_HOST) -fno-common -o $@ $(ROUTE_SIM) route/snap.c vs_cpu.c route/vs_route.c -lm
# Vs CPU racing lines (disc build: tools/dreamcast/disc/assemble_disc.py -> /cd/vs): one per stage and
# character, made from the converted stages with the real simulation (route/vs_route.c); a line that fails
# is reported and left out (the CPU then drives by sight on that stage)
ROUTE_BUDGET ?= 900
ROUTES := $(foreach s,$(CPU_STAGES),$(foreach c,0 1,$(OUT)/routes/$(s)_$(c).rte))
routes: $(ROUTES)
$(OUT)/routes/%.rte: $(ROOT)/build/dreamcast/stages/manifest.json | $(OUT)/vs_route
	@mkdir -p $(OUT)/routes
	@cd $(ROOT)/build/dreamcast && { ./vs/vs_route stages player objects sprites/bac $(word 1,$(subst _, ,$*)) $(word 2,$(subst _, ,$*)) \
	    vs/routes/$*.rte.tmp -budget $(ROUTE_BUDGET); rc=$$?; \
	    if [ $$rc = 0 ] || [ $$rc = 3 ]; then mv vs/routes/$*.rte.tmp vs/routes/$*.rte; \
	    else echo "vs routes: $* not found (the CPU follows the rival there)"; fi; }
.PHONY: all host-test cpu-test routes
