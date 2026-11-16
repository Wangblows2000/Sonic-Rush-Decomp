#include <nitro/types.h>

typedef struct UnkStruct_02348ca8 UnkStruct_02348ca8;
struct UnkStruct_02348ca8 {
    u8 unk0;
    u8 unk1;
    u8 pad2[5 - 2];
    u8 unk5;
    u8 pad6[8 - 6];
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
    u16 unk5E;
    u16 unk60;
    u8 pad62[0x68 - 0x62];
    void (*state)(UnkStruct_02348ca8 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3DC - 0x384];
    UnkStruct_02348ca8 *target;
    u8 pad3E0[0x3E8 - 0x3E0];
    UnkStruct_02348ca8 *other;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
};

void func_ov000_02348b28(UnkStruct_02348ca8 *w);
void func_ov000_0234a878(UnkStruct_02348ca8 *w);
void func_ov000_0234c4a4(UnkStruct_02348ca8 *w, s32 a);
void ObjRect_SetAtkDefFlags(void *p, s32 a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
u16 func_020027a4(s32 y, s32 x);

void func_ov000_02348ca8(UnkStruct_02348ca8 *w) {
    s32 dx;
    s32 dy;
    u16 ang;
    s32 d;

    if (w->target == NULL) {
        func_ov000_0234a878(w);
        return;
    }
    dx = (w->target->unk10 - w->unk10) >> 8;
    dy = (w->target->unk14 - w->unk14) >> 8;
    ang = func_020027a4(dy, dx) + (w->unk5 << 8);
    d = dx * dx + dy * dy;
    if (ang >= 0x8000 || d >= 0x8000) {
        func_ov000_0234a878(w);
        return;
    }
    func_ov000_0234c4a4(w, 0x30);
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk58 &= ~0x80;
    w->unk32C |= 0x40;
    w->unk1 = 0;
    w->state = func_ov000_02348b28;
    w->unkC = 0x20;
    ObjRect_SetAtkDefFlags(w->unk2CC, 0x13, 0);
    w->unk32C &= ~3;
    Sound_PlaySeqArcEx(w->unk384, 0, 7);
}
