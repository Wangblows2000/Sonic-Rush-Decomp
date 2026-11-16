#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[3];
    u8 unk5;
    u8 pad6[0x10 - 6];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x28 - 0x1C];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x58 - 0x2C];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    u16 unk62;
    u8 pad64[0x68 - 0x64];
    void *unk68;
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u32 unk330;
} UnkStruct_02331d44_P;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xC - 2];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x2E - 0x1C];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_02331d44_P *unk88;
} UnkStruct_02331d44;

void func_ov000_02351a8c(void);
u16 func_020027a4(s32 y, s32 x);
void StageObj_ReleaseSpriteVram(void *w);

void func_ov000_02331d44(UnkStruct_02331d44 *w) {
    UnkStruct_02331d44_P *p = w->unk88;

    if (p == NULL)
        return;
    w->unk10 = p->unk10;
    w->unk14 = w->unk88->unk14;
    w->unk18 = w->unk88->unk18;
    if (p->unk68 == func_ov000_02351a8c) {
        w->unk5C &= ~1;
        if (p->unk5C & 1)
            w->unk1 = w->unk88->unk1 + 0x80;
        else
            w->unk1 = w->unk88->unk1;
        w->unk1 += w->unk88->unk5;
    } else {
        if (!(w->unk88->unk62 & 0xf)) {
            if (w->unk88->unk2A == 0 && w->unk88->unk28 == 0)
                w->unk1 = w->unk88->unk1;
            else
                w->unk1 = func_020027a4(w->unk88->unk2A, w->unk88->unk28) >> 8;
            if (w->unk1 == 0 && w->unk88->unk5 == 0 && w->unk88->unk1 == 0 && (w->unk88->unk5C & 1))
                w->unk1 += 0x80;
        }
        w->unk5C &= ~1;
    }
    w->unk10 += w->unk2E << 8;
    w->unk14 += w->unk30 << 8;
    if (p->unk330 & 0x20000)
        w->unk5C |= 0x20;
    else
        w->unk5C &= ~0x20;
    if (w->unkC != 0) {
        if (!(w->unk88->unk58 & 1) && w->unkC > 8)
            w->unkC = 8;
        w->unkC--;
        if ((p->unk32C & 0x100) && w->unkC != 0)
            return;
        w->unk54 |= 4;
        StageObj_ReleaseSpriteVram(w);
    } else {
        if (p->unk32C & 0x80)
            return;
        w->unk54 |= 4;
        StageObj_ReleaseSpriteVram(w);
    }
}
