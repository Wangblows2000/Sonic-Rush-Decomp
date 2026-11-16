#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0xc - 4];
    s32 unkC;
    u8 pad10[0x18 - 0x10];
    s32 unk18;
    u8 pad1C[0x28 - 0x1c];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x2e - 0x2c];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5e];
    void *unk68;
    u8 pad6C[0x32c - 0x6c];
    u32 unk32C;
    u8 pad330[0x388 - 0x330];
    u8 unk388[0x58];
} UnkStruct_02352fc0;

void func_ov000_02352ee0(void);
s32 func_ov000_0234cb70();
s32 Player_SetAnimation();
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void func_ov000_02352fc0(UnkStruct_02352fc0 *w, s32 x, s32 y) {
    u8 b3 = w->unk3;
    s32 v18 = w->unk18;

    if (w->unk68 == func_ov000_02352ee0) {
        w->unk2E = (w->unk28 + x) >> 1;
        w->unk30 = (w->unk2A + y) >> 1;
        w->unkC = 0;
        w->unk58 &= ~0x80;
        return;
    }
    func_ov000_0234cb70(w);
    Player_SetAnimation(w, 0x29);
    w->unk5C |= 4;
    w->unk68 = func_ov000_02352ee0;
    w->unk58 |= 0x10;
    w->unk58 &= ~0x80;
    w->unk32C |= 0x100000;
    w->unk2E = (w->unk28 + x) >> 1;
    w->unk30 = (w->unk2A + y) >> 1;
    w->unk34 = 0;
    w->unk32 = 0;
    w->unk1 = 0;
    w->unk2 = 0;
    w->unk3 = b3;
    w->unk18 = v18;
    Sound_PlaySeqArcEx(w->unk388, 0, 0x58);
}
