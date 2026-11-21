#include <nitro/types.h>

typedef struct {
    u8 pad0[0x84];
    void *unk84;
    void *unk88;
} UnkStruct_0233ce78_Vram;

typedef struct {
    u8 pad0[0x2AC];
    u8 unk2AC[0x6AC - 0x2AC];
    u8 unk6AC[0x8B0 - 0x6AC];
    u8 *unk8B0;
} UnkStruct_0233ce78;

extern UnkStruct_0233ce78_Vram gDisplayMain;
extern UnkStruct_0233ce78_Vram gDisplaySub;

void DC_StoreRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void MI_DmaCopy16(u32 ch, const void *src, void *dst, u32 size);
s32 GX_ResetBankForBGExtPltt(void);
void GX_SetBankForBGExtPltt(s32 a);
s32 GX_ResetBankForSubBGExtPltt(void);
void GX_SetBankForSubBGExtPltt(s32 a);

void ZoneFx_Zone2_DmaTintedPalettes(UnkStruct_0233ce78 *w, s32 sub) {
    s32 st;
    u8 *p;

    DC_StoreRange(w, 0x950);
    DC_WaitWriteBufferEmpty();
    if (sub == 0) {
        MI_DmaCopy16(3, w->unk6AC, (void *)0x5000000, 0x1e0);
        st = GX_ResetBankForBGExtPltt();
        p = w->unk2AC;
        MI_DmaCopy16(3, p, gDisplayMain.unk84, 0x1e0);
        MI_DmaCopy16(3, p, gDisplayMain.unk88, 0x1e0);
        GX_SetBankForBGExtPltt(st);
        MI_DmaCopy16(3, w->unk8B0, (void *)0x5000200, 0);
        MI_DmaCopy16(3, w->unk8B0 + 0x40, (void *)0x5000240, 0x1c0);
    } else {
        MI_DmaCopy16(3, w->unk6AC, (void *)0x5000400, 0x1e0);
        st = GX_ResetBankForSubBGExtPltt();
        p = w->unk2AC;
        MI_DmaCopy16(3, p, gDisplaySub.unk84, 0x1e0);
        MI_DmaCopy16(3, p, gDisplaySub.unk88, 0x1e0);
        GX_SetBankForSubBGExtPltt(st);
        MI_DmaCopy16(3, w->unk8B0, (void *)0x5000600, 0);
        MI_DmaCopy16(3, w->unk8B0 + 0x40, (void *)0x5000640, 0x1c0);
    }
}
