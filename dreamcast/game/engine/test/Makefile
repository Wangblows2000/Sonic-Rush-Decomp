# Host test of the palette RAM allocator / write queue in engine/render.c (KOS stubs in stub/)
OUT ?= ../../../../build/dreamcast/engine
all: $(OUT)/test_palette
	$(OUT)/test_palette
$(OUT)/test_palette: test_palette.c ../render.c ../render.h stub/kos.h stub/dc/pvr.h
	mkdir -p $(OUT)
	gcc -std=gnu99 -O1 -Wall -Wextra -Wno-unused-parameter -Istub -o $@ test_palette.c ../render.c
.PHONY: all
