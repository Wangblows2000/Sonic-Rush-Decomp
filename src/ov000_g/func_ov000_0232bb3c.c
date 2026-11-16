#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x14 - 0x10];
    s32 unk14;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x9C - 0x5E];
    u16 unk9C;
    u8 pad9E[0xC0 - 0x9E];
    u32 unkC0;
    u8 padC4[0x108 - 0xC4];
    u32 unk108;
} UnkStruct_0232bb3c;

typedef struct {
    u8 pad0[0xC8];
    u32 unkC8;
} UnkStruct_0232bb3c_S;

extern UnkStruct_0232bb3c_S gCamera;

void StageObj_SetAnimation(void *w, u16 a);

void func_ov000_0232bb3c(UnkStruct_0232bb3c *w) {
    w->unkC0 &= ~0x80000;
    w->unk108 &= ~0x80000;
    if (w->unkC != 0) {
        w->unkC--;
        if (w->unkC == 0)
            w->unk58 &= ~0x2000;
    }
    if (gCamera.unkC8 & 0x200)
        w->unk8 ^= 1;
    if (w->unk8 != 0)
        w->unkC0 |= 0x80000;
    else
        w->unk108 |= 0x80000;
    if (w->unk30 >= -0x180 && (w->unk5C & 4))
        StageObj_SetAnimation(w, w->unk9C + 1);
    if (w->unk30 <= 0)
        return;
    if ((w->unk14 >> 8) > 0xe0)
        w->unk54 |= 4;
}
