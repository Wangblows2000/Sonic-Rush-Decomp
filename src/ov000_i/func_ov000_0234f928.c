#include <nitro/types.h>

typedef struct UnkStruct_0234f928 UnkStruct_0234f928;
struct UnkStruct_0234f928 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x8 - 2];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x14 - 0x10];
    s32 unk14;
    u8 pad18[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5e];
    void *unk68;
    u8 pad6C[0x32a - 0x6c];
    s16 unk32A;
    u8 pad32C[0x3cf - 0x32c];
    u8 unk3CF;
    u8 pad3D0;
    u8 unk3D1;
    u8 pad3D2[0x3da - 0x3d2];
    s16 unk3DA;
    u8 pad3DC[0x3e8 - 0x3dc];
    UnkStruct_0234f928 *unk3E8;
};

void func_ov000_0234f7ac(void);
void func_ov000_0234c4a4(UnkStruct_0234f928 *w, s32 a);

void func_ov000_0234f928(UnkStruct_0234f928 *w) {
    w->unk58 |= 0x200000;
    w->unk58 |= 0x80;
    w->unk58 &= ~0x10000000;
    w->unk58 &= ~0x100;
    w->unk58 &= ~0x2000;
    if (!(w->unk58 & 0x10))
        w->unk3DA = w->unk14 >> 8;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk58 &= ~1;
    w->unk68 = func_ov000_0234f7ac;
    func_ov000_0234c4a4(w, 0x18);
    w->unk5C |= 4;
    w->unk2E = w->unk3E8->unk2E << 1;
    w->unk30 = -0x680;
    w->unk1 = 0;
    w->unkC = 0;
    w->unk8 = 0;
    w->unk32A = 0;
    w->unk3CF = 0;
    w->unk3D1 = 8;
}
