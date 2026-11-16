#include <nitro/types.h>

typedef struct UnkStruct_0234f7ac {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x30 - 0x10];
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x328 - 0x5E];
    s16 unk328;
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x334 - 0x330];
    void (*unk334)(struct UnkStruct_0234f7ac *w);
    u8 pad338[0x422 - 0x338];
    u16 unk422;
} UnkStruct_0234f7ac;

void func_ov000_023497f4(UnkStruct_0234f7ac *w);
void func_ov000_0234c4a4(UnkStruct_0234f7ac *w, s32 a);
void func_ov000_0234a064(UnkStruct_0234f7ac *w, u8 a);

void func_ov000_0234f7ac(UnkStruct_0234f7ac *w) {
    s16 vy = w->unk30;

    if (w->unk422 & 3) {
        w->unk8 = w->unk422;
    }
    if (w->unk328 >= 0x39 && w->unk328 <= 0x3b) {
        w->unk32A++;
        if (w->unk32A > 0xff) {
            w->unk32A = 0xff;
        }
    }
    if (!(w->unk32C & 4) && (w->unk328 < 0x39 || w->unk32A > 0x18) && w->unk8 != 0) {
        func_ov000_023497f4(w);
    }
    if (w->unkC != 0 && vy > -0x30) {
        u32 *fl;
        w->unk30 = 0;
        fl = &w->unk58;
        w->unk58 |= 0x2000;
        w->unk58 &= ~0x80;
        w->unkC--;
        if (w->unkC == 0) {
            *fl &= ~0x2000;
            *fl |= 0x80;
        }
    }
    if (w->unk328 == 0x18 && vy > 0x40) {
        w->unk5C |= 0x400;
        func_ov000_0234c4a4(w, 0x19);
        w->unk5C |= 4;
    }
    if (!(w->unk58 & 1)) {
        return;
    }
    w->unk32C &= ~0x100000;
    func_ov000_0234a064(w, 0);
    w->unk334(w);
}
