#include <nitro/types.h>

typedef struct UnkStruct_0234f4ec UnkStruct_0234f4ec;
struct UnkStruct_0234f4ec {
    u8 unk0;
    u8 unk1;
    u8 pad2[8 - 2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*state)(UnkStruct_0234f4ec *w);
    u8 pad6C[0x324 - 0x6C];
    u8 unk324;
    u8 pad325[0x32C - 0x325];
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_0234f4ec *other;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
};

void func_ov000_0234f3b4(UnkStruct_0234f4ec *w);
void func_ov000_0234f054(UnkStruct_0234f4ec *w);
void func_ov000_0234cb70(UnkStruct_0234f4ec *w);
void func_ov000_0234c4a4(UnkStruct_0234f4ec *w, s32 a);
void func_0204b7ec(void *p, s32 a, s32 b);

void func_ov000_0234f4ec(UnkStruct_0234f4ec *w, UnkStruct_0234f4ec *other, s32 v) {
    if (w->other == other)
        return;
    if (w->state == func_ov000_0234f3b4)
        return;
    if (w->state == func_ov000_0234f054)
        return;
    func_ov000_0234cb70(w);
    w->other = other;
    func_ov000_0234c4a4(w, 0x31);
    w->unk5C |= 4;
    w->state = func_ov000_0234f3b4;
    w->unk5C &= ~1;
    w->unk5C |= other->unk5C & 1;
    w->unk5C ^= 1;
    w->unk58 |= 0x10;
    w->unk58 &= ~0x80;
    w->unk32C &= ~1;
    w->unk34 = 0;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk1 = 0;
    w->unk8 = 0;
    w->unkC = 0;
    w->unk3F0 = v;
    w->unk3F4 = w->unk10;
    w->unk3F8 = w->unk14;
    if (w->unk324 != 1) {
        func_0204b7ec(w->unk384, 0, 0);
    } else {
        func_0204b7ec(w->unk384, 0, 0x11);
    }
}
