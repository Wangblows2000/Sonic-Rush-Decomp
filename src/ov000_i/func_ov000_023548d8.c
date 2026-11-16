#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xc - 2];
    s32 unkC;
    u8 pad10[0x2e - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5e];
    void *unk68;
    u8 pad6C[0x2cc - 0x6c];
    u8 unk2CC[0x2d6 - 0x2cc];
    u16 unk2D6;
    u8 pad2D8[0x2e8 - 0x2d8];
    s32 unk2E8;
    u8 pad2EC[0x3f4 - 0x2ec];
    u32 unk3F4;
} UnkStruct_023548d8;

void func_ov000_023548a4(void);
s32 func_ov000_0234c9e8();
void func_ov000_0234c4a4(UnkStruct_023548d8 *w, s32 a);
void func_0206e2b4(void *p, s32 a, s32 b);

void func_ov000_023548d8(UnkStruct_023548d8 *w, s32 a, s32 b) {
    func_ov000_0234c9e8(w);
    func_ov000_0234c4a4(w, 0x31);
    w->unk5C |= 4;
    w->unk58 &= ~0x10;
    w->unk58 |= 0x8000;
    w->unk68 = func_ov000_023548a4;
    w->unk3F4 = (w->unk2E << 16) | (a & 0xffff);
    w->unkC = b;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk1 = 0;
    func_0206e2b4(w->unk2CC, 0x13, 0);
    w->unk2E8 = 0;
    w->unk2D6 &= ~8;
}
