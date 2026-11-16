#include <nitro/types.h>

typedef struct {
    u8 pad0[0xAC];
    u8 unkAC[0x200];
    u8 unk2AC[0x200];
    u8 unk4AC[0x200];
    u8 unk6AC[0x200];
    u8 *unk8AC;
    u8 *unk8B0;
} UnkStruct_0233cfd8;

void PaletteTransfer_QueueCopy(void *src, s32 a, s32 b, u32 dst);

void func_ov000_0233cfd8(UnkStruct_0233cfd8 *w, s32 i) {
    u8 *p;

    if (i == 0) {
        PaletteTransfer_QueueCopy(w->unk4AC, 0xF0, 0, 0x5000000);
        p = w->unkAC;
        PaletteTransfer_QueueCopy(p, 0xF0, 1, 0x4000);
        PaletteTransfer_QueueCopy(p, 0xF0, 1, 0x6000);
        PaletteTransfer_QueueCopy(w->unk8AC, 0, 0, 0x5000200);
        PaletteTransfer_QueueCopy(w->unk8AC + 0x40, 0xE0, 0, 0x5000240);
    } else {
        PaletteTransfer_QueueCopy(w->unk4AC, 0xF0, 0, 0x5000400);
        p = w->unkAC;
        PaletteTransfer_QueueCopy(p, 0xF0, 3, 0x4000);
        PaletteTransfer_QueueCopy(p, 0xF0, 3, 0x6000);
        PaletteTransfer_QueueCopy(w->unk8AC, 0, 0, 0x5000600);
        PaletteTransfer_QueueCopy(w->unk8AC + 0x40, 0xE0, 0, 0x5000640);
    }
}
