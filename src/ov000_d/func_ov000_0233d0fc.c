#include <nitro/types.h>

typedef struct {
    u8 pad0[0x84];
    void *unk84;
    void *unk88;
} UnkStruct_0233d0fc_Vram;

typedef struct {
    u8 pad0[0xAC];
    u8 unkAC[0x4AC - 0xAC];
    u8 unk4AC[0x8AC - 0x4AC];
    u8 *unk8AC;
} UnkStruct_0233d0fc;

extern UnkStruct_0233d0fc_Vram gDisplayMain;
extern UnkStruct_0233d0fc_Vram gDisplaySub;

void DC_StoreRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void MI_DmaCopy16(u32 ch, const void *src, void *dst, u32 size);
s32 GX_ResetBankForBGExtPltt(void);
void GX_SetBankForBGExtPltt(s32 a);
s32 GX_ResetBankForSubBGExtPltt(void);
void GX_SetBankForSubBGExtPltt(s32 a);

void func_ov000_0233d0fc(UnkStruct_0233d0fc *w, s32 sub) {
    s32 st;
    u8 *p;

    DC_StoreRange(w, 0x950);
    DC_WaitWriteBufferEmpty();
    if (sub == 0) {
        MI_DmaCopy16(3, w->unk4AC, (void *)0x5000000, 0x1e0);
        st = GX_ResetBankForBGExtPltt();
        p = w->unkAC;
        MI_DmaCopy16(3, p, gDisplayMain.unk84, 0x1e0);
        MI_DmaCopy16(3, p, gDisplayMain.unk88, 0x1e0);
        GX_SetBankForBGExtPltt(st);
        MI_DmaCopy16(3, w->unk8AC, (void *)0x5000200, 0);
        MI_DmaCopy16(3, w->unk8AC + 0x40, (void *)0x5000240, 0x1c0);
    } else {
        MI_DmaCopy16(3, w->unk4AC, (void *)0x5000400, 0x1e0);
        st = GX_ResetBankForSubBGExtPltt();
        p = w->unkAC;
        MI_DmaCopy16(3, p, gDisplaySub.unk84, 0x1e0);
        MI_DmaCopy16(3, p, gDisplaySub.unk88, 0x1e0);
        GX_SetBankForSubBGExtPltt(st);
        MI_DmaCopy16(3, w->unk8AC, (void *)0x5000600, 0);
        MI_DmaCopy16(3, w->unk8AC + 0x40, (void *)0x5000640, 0x1c0);
    }
}
