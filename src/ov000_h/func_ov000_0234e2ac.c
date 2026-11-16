#include <nitro/types.h>

typedef struct UnkStruct_0234e2ac UnkStruct_0234e2ac;
struct UnkStruct_0234e2ac {
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
    void (*state)(UnkStruct_0234e2ac *w);
    u8 pad6C[0x324 - 0x6C];
    u8 unk324;
    u8 pad325[0x32C - 0x325];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x384 - 0x334];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_0234e2ac *other;
    u8 pad3EC[0x3EE - 0x3EC];
    s16 unk3EE;
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
};

typedef struct {
    u8 pad0[0x45];
    u8 unk45;
} UnkStruct_0234e2ac_4590;

extern UnkStruct_0234e2ac_4590 data_022c4590;

void func_ov000_0232f5a8(UnkStruct_0234e2ac *w);
void func_ov000_0234e0f8(UnkStruct_0234e2ac *w);
void func_ov000_0234c9e8(UnkStruct_0234e2ac *w);
void func_ov000_0234c4a4(UnkStruct_0234e2ac *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void func_ov000_0234e2ac(UnkStruct_0234e2ac *w, UnkStruct_0234e2ac *other, BOOL flip) {
    if (w->other == other)
        return;
    if (data_022c4590.unk45 == 0) {
        func_ov000_0232f5a8(w);
        data_022c4590.unk45 = 1;
    }
    func_ov000_0234c9e8(w);
    w->other = other;
    func_ov000_0234c4a4(w, 0x25);
    w->unk5C |= 4;
    w->state = func_ov000_0234e0f8;
    w->unk58 |= 0x10;
    w->unk58 |= 0x2000;
    if (flip)
        w->unk5C |= 1;
    else
        w->unk5C &= ~1;
    w->unk32C |= 0x100000;
    w->unk32C &= ~0xF;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk34 = 0;
    w->unk1 = 0;
    w->unk8 = 0;
    w->unkC = 0;
    Sound_PlaySeqArcEx(NULL, 0, 0xEC);
    w->unk32C |= 0x2000;
    w->unk330 |= 0x20;
    w->unk3EE = -0x3A;
}
