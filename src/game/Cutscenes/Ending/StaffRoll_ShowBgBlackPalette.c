#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x10];
    s32 unk14;
    s32 unk18;
    u8 pad1c[0x14];
    void *unk30;
} UnkStruct_022cff74;

extern void *gPaletteTransferHead;
extern void *gPaletteTransferTail;

void Background_Draw(UnkStruct_022cff74 *p);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void PaletteTransfer_QueueLoad(void *p, u32 a, u32 b);
void PaletteTransfer_Flush(void);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, s32 c);

#pragma thumb on
static inline void LoadPal_022cff74(void *src, u32 a, u32 b) {
    void *s0 = gPaletteTransferHead;
    void *s1 = gPaletteTransferTail;
    gPaletteTransferHead = NULL;
    gPaletteTransferTail = NULL;
    PaletteTransfer_QueueLoad(src, a, b);
    PaletteTransfer_Flush();
    gPaletteTransferHead = s0;
    gPaletteTransferTail = s1;
}

void StaffRoll_ShowBgBlackPalette(UnkStruct_022cff74 *p, u32 n, u32 b, void *buf) {
    p->flags &= ~0x38;
    Background_Draw(p);
    {
        vu32 z = 0;
        MIi_CpuClear32(z, buf, n * 2);
    }
    LoadPal_022cff74(p->unk30, 0, b);
    PaletteTransfer_QueueCopy(buf, (u16)n, p->unk14, p->unk18);
}
#pragma thumb reset
