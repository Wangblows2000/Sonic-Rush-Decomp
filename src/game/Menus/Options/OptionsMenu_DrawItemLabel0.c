#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1d08];
    u8 tiles[7][0x20];
    u32 vram[7];
} UnkWork_DrawItemLabel0;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_DrawItemLabel0 *work;
} UnkStruct_DrawItemLabel0;

typedef struct {
    u8 pad0[0x14];
    u16 shift;
} UnkGx_DrawItemLabel0;

extern u32 gObjVramBaseTable[];
extern UnkGx_DrawItemLabel0 data_02087d84;
extern void *gVRAMTransferHead;
extern void *gVRAMTransferTail;
extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;

void PaletteTransfer_QueueCopy(void *src, s32 a, s32 b, u32 dst);
void PaletteTransfer_Flush(void);
void VRAMTransfer_QueueCopy(void *src, s32 size, s32 a, u32 dst);
void VRAMTransfer_Flush(void);
u16 *Oam_AllocObj(s32 a, s32 b);

static inline void DrawObj_DrawItemLabel0(s32 px, s32 py, u32 vram, u32 base) {
    u16 *oam;
    if (px > -8 && px < 0x100 && py > -8 && py < 0xc0) {
        oam = Oam_AllocObj(1, 0);
        u32 a2 = (vram - base) >> (data_02087d84.shift + 5);
        oam[0] = (u8)py;
        oam[1] = (u8)px;
        oam[2] = (a2 & 0x3ff) | 0xc800;
    }
}


#pragma thumb on
#pragma opt_loop_invariants off
void OptionsMenu_DrawItemLabel0(UnkStruct_DrawItemLabel0 *p, s32 x, s32 y, BOOL alt) {
    u32 vram;
    s32 i;
    s32 py;
    s32 px;
    UnkWork_DrawItemLabel0 *w;
    u16 *oam;
    u32 tile;
    u16 col;
    u32 sa;
    u32 sb;
    u32 voff;
    void *pa;
    void *pb;
    u32 base;

    x -= 0x48;
    py = y + 7;
    if (alt)
        col = 0;
    else
        col = 0x318c;
    sa = gPaletteTransferHead;
    sb = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueCopy(&col, 1, 0, 0x500079e);
    PaletteTransfer_Flush();
    gPaletteTransferHead = sa;
    gPaletteTransferTail = sb;

    for (i = 0, tile = 0, voff = 0, base = gObjVramBaseTable[1]; i < 7; tile += 0x20, voff += 4, i++) {
        w = p->work;
        vram = *(u32 *)((u8 *)w->vram + voff);
        {
            pa = gVRAMTransferHead;
            pb = gVRAMTransferTail;
            gVRAMTransferHead = NULL;
            gVRAMTransferTail = NULL;
            VRAMTransfer_QueueCopy(w->tiles[0] + tile, 0x20, 0, vram);
            VRAMTransfer_Flush();
            gVRAMTransferHead = pa;
            gVRAMTransferTail = pb;
        }
        DrawObj_DrawItemLabel0(x, py, vram, base);
        x += 8;
    }
}
#pragma thumb reset
