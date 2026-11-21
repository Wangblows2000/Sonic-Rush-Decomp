#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_02320be8_P;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x9C - 0x6C];
    u16 unk9C;
} UnkStruct_02320be8;

extern s16 data_0206e470[];

void StageObj_SetAnimation(void *w, u16 a);
u16 FX_Atan2(s32 y, s32 x);
void DecFishPart_State_Flee(void);

void DecFishPart_Action_Flee(UnkStruct_02320be8 *w, UnkStruct_02320be8_P *p) {
    s32 idx;

    if (w->unk9C & 1)
        StageObj_SetAnimation(w, w->unk9C - 1);
    w->unk68 = DecFishPart_State_Flee;
    w->unk5C |= 4;
    w->unk54 |= 2;
    w->unk58 &= ~0x80;
    w->unk58 &= ~0x2000;
    idx = (s32)(FX_Atan2(w->unk14 - p->unk14, w->unk10 - p->unk10) & 0xffff) >> 4;
    w->unk2E = (data_0206e470[(u32)((idx << 1) + 1)] << 10) >> 12;
    w->unk30 = (data_0206e470[(u32)(idx << 1)] << 10) >> 12;
    if (w->unk2E > 0)
        w->unk5C &= ~1;
    else
        w->unk5C |= 1;
}
