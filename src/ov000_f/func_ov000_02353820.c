#include <nitro/types.h>

typedef struct UnkStruct_02353820 UnkStruct_02353820;

struct UnkStruct_02353820 {
    u8 pad0;
    u8 unk1;
    u8 pad2;
    u8 unk3;
    u8 unk4;
    u8 pad5[0x10 - 5];
    s32 x;
    s32 y;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x3C - 0x36];
    s16 unk3C;
    u8 pad3E[0x58 - 0x3E];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(UnkStruct_02353820 *w);
    u8 pad6C[0x324 - 0x6C];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x388 - 0x330];
    u8 unk388[0x3F0 - 0x388];
    s32 unk3F0;
};

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_02353820;

typedef struct {
    s16 v;
    u8 pad2[0x30 - 2];
} UnkPhys_02353820;

extern UnkPhys_02353820 data_ov000_02368ba0[];

void func_ov000_02353568(UnkStruct_02353820 *w);
void func_ov000_0234c9e8(UnkStruct_02353820 *w);
void func_ov000_0234a064(UnkStruct_02353820 *w, u8 ang);
void func_ov000_0234c4a4(UnkStruct_02353820 *w, s32 state);
void func_0204b7ec(void *p, s32 a, s32 b);

void func_ov000_02353820(UnkStruct_02353820 *w, UnkObj_02353820 *o) {
    if (w->unk68 == func_ov000_02353568) {
        if (w->unk3F0 != 0) {
            return;
        }
        if (w->unk5C & 1) {
            w->unk3F0 = w->x - (o->x + 0x3e00);
            if (w->unk3F0 < 0) {
                w->unk3F0 = 8;
            }
        } else {
            w->unk3F0 = (o->x - 0x1000) - w->x;
            if (w->unk3F0 > 0) {
                w->unk3F0 = -1;
            }
        }
        return;
    }
    func_ov000_0234c9e8(w);
    func_ov000_0234a064(w, 0);
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk1 = 0;
    w->unk3 = 0;
    func_ov000_0234c4a4(w, 0x27);
    if (w->unk34 > 0) {
        w->unk5C &= ~1;
    } else {
        w->unk5C |= 1;
    }
    if (w->unk5C & 1) {
        if (w->unk34 > -0x200) {
            w->unk34 = -0x200;
        }
    } else {
        if (w->unk34 < 0x200) {
            w->unk34 = 0x200;
        }
    }
    w->x = o->x;
    w->y = o->y + 0x200;
    w->unk3F0 = 0;
    w->unk58 &= ~0x80;
    w->unk58 |= 0x2000;
    w->unk58 &= ~0x20000;
    w->unk32C |= 0x100000;
    w->unk3C = data_ov000_02368ba0[w->unk324].v;
    w->unk4 = 0xe;
    w->unk68 = func_ov000_02353568;
    w->unk32A = 8;
    w->unk58 &= ~0xf;
    func_0204b7ec(w->unk388, 0, 0x55);
}
