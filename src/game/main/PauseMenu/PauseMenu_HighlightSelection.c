#include <nitro/types.h>

typedef struct UnkEntry_020551b0 {
    u8 pad0[0x10];
    void *vram;
    u8 pad14[0x48 - 0x14];
} UnkEntry_020551b0;

typedef struct UnkPauseMenu_020551b0 {
    s16 cur;
    u8 pad2[0x44 - 0x2];
    UnkEntry_020551b0 entries[4];
    u8 pad[0x17a - 0x44 - 4 * 0x48];
    u16 palette[2][0x100];
    u16 work[0x100];
} UnkPauseMenu_020551b0;

extern u8 data_0207ba24[];

void VRAMTransfer_QueueCopy(void *src, u32 size, u32 unk, void *dst);

static inline int Lut_020551b0(s32 i) { return data_0207ba24[i]; }

void PauseMenu_HighlightSelection(UnkPauseMenu_020551b0 *menu)
{
    u16 *src;
    u16 *dst;
    s16 a;
    s16 b;
    s16 cur;
    s32 other;
    u16 i;
    u16 p;
    u16 c;

    cur = menu->cur;
    other = cur ^ 1;
    a = cur + 2;
    b = other + 2;
    src = menu->palette[other];
    dst = menu->work;
    for (i = 0; i < 0x100; i++) {
        p = *src;
        c = 0;
        c |= Lut_020551b0(p & 0xf);
        c |= Lut_020551b0((s32)(p & 0xf0) >> 4) << 4;
        c |= Lut_020551b0((s32)(p & 0xf00) >> 8) << 8;
        c |= Lut_020551b0((s32)(p & 0xf000) >> 12) << 12;
        *dst = c;
        src++;
        dst++;
    }
    VRAMTransfer_QueueCopy(menu->palette[menu->cur], 0x200, 0, menu->entries[a].vram);
    VRAMTransfer_QueueCopy(menu->work, 0x200, 0, menu->entries[b].vram);
}
