#include <nitro/types.h>

typedef struct UnkStruct_02352b14 {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 pad24[4];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x384 - 0x334];
    u8 unk384[0x3E8 - 0x384];
    struct UnkStruct_02352b14 *unk3E8;
    u8 pad3EC[4];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
} UnkStruct_02352b14;

void func_ov000_023528d4(void);
void func_ov000_0234c9e8(UnkStruct_02352b14 *w);
void func_ov000_0234a064(UnkStruct_02352b14 *w, u8 a);
void func_ov000_0234c4a4(UnkStruct_02352b14 *w, s32 a);
void func_0204b7ec(void *p, s32 a, s32 b);

void func_ov000_02352b14(UnkStruct_02352b14 *w, UnkStruct_02352b14 *o, s32 x, s32 y) {
    if (w->unk68 == func_ov000_023528d4) {
        return;
    }
    if (w->unk58 & 1) {
        w->unk2E = w->unk34;
    }
    func_ov000_0234c9e8(w);
    func_ov000_0234a064(w, 0);
    w->unk3E8 = o;
    func_ov000_0234c4a4(w, 0x2a);
    w->unk5C |= 4;
    w->unk68 = func_ov000_023528d4;
    w->unk58 |= 0x10;
    w->unk58 |= 0x2000;
    w->unk58 |= 0x100;
    w->unk58 &= ~0x80;
    w->unk58 &= ~0x40;
    w->unk32C |= 0x100000;
    w->unk14 = w->unk3E8->unk14 - 0xc00;
    w->unk3F0 = 0;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk28 = 0;
    w->unk2A = 0;
    w->unk34 = 0;
    w->unk1 = 0;
    w->unk8 = 0;
    w->unkC = 0;
    w->unk3F4 = 0;
    w->unk3F8 = o->unk10 + (x << 8) - w->unk10;
    w->unk3FC = o->unk14 + (y << 8) - w->unk14;
    w->unk1C = w->unk10;
    w->unk20 = w->unk14;
    w->unk10 = w->unk3E8->unk10 + w->unk3F0;
    w->unk14 = w->unk3E8->unk14 + w->unk3F4;
    w->unk32C |= 0x2000;
    w->unk330 |= 0x30;
    w->unk330 |= 0x20000;
    func_0204b7ec(w->unk384, 0, 0x5d);
}
