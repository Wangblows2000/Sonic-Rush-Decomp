#include <nitro/types.h>

typedef struct UnkEntry_020551b0 {
    u8 pad0[0xc];
    void *vram;
    u8 pad10[0x48 - 0x10];
} UnkEntry_020551b0;

typedef struct UnkPauseMenu_020551b0 {
    s16 cur;
    u8 pad2[0x48 - 0x2];
    UnkEntry_020551b0 entries[4];
    u8 pad[0x17a - 0x48 - 4 * 0x48];
    u16 palette[2][0x100];
    u16 work[0x100];
} UnkPauseMenu_020551b0;

extern u8 data_0207ba24[];

void VRAMTransfer_QueueCopy(void *src, u32 size, u32 unk, void *dst);

void PauseMenu_HighlightSelection(UnkPauseMenu_020551b0 *menu)
{
    s16 cur = menu->cur;
    s32 other = cur ^ 1;
    s16 a = cur + 2;
    s16 b = other + 2;
    u16 *src = menu->palette[other];
    u16 *dst = menu->work;
    u16 i = 0;

    do {
        u16 p = *src;
        u16 c = 0;
        i++;
        c = c | (u16)data_0207ba24[p & 0xf];
        c |= (data_0207ba24[(s32)(p & 0xf0) >> 4] << 4);
        c |= (data_0207ba24[(s32)(p & 0xf00) >> 8] << 8);
        c = c | (u16)(data_0207ba24[(s32)(p & 0xf000) >> 12] << 12);
        *dst = c;
        src++;
        dst++;
    } while (i < 0x100);
    VRAMTransfer_QueueCopy(menu->palette[menu->cur], 0x200, 0, menu->entries[a].vram);
    VRAMTransfer_QueueCopy(menu->work, 0x200, 0, menu->entries[b].vram);
}
