#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    u8 pad30[4];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u8 pad5C[0x64 - 0x5C];
    s8 unk64;
    u8 pad65;
    s8 unk66;
    s8 unk67;
} UnkStruct_023526cc;

extern u32 gObjManagerFlags;
extern s16 data_0206e470[];

static inline s16 UnkSin_023526cc(u16 a) {
    s32 i = (s32)(a & 0xffff) >> 4;
    return data_0206e470[(u32)(i << 1)];
}

void func_ov000_023526cc(UnkStruct_023526cc *w, UnkStruct_023526cc *o) {
    s32 lim = -0x1200;
    s32 v;

    if (o == NULL) {
        return;
    }
    if (w->unk10 + (w->unk64 << 8) <= o->unk10 - 0x6000) {
        w->unk10 = o->unk10 - 0x6000 - (w->unk64 << 8);
        if (w->unk34 < 0) {
            w->unk34 = 0;
        }
        if (w->unk2E < 0) {
            w->unk2E = 0;
        }
        w->unk58 |= 4;
    }
    if (w->unk10 + (w->unk66 << 8) >= o->unk10 + 0x6000) {
        w->unk10 = o->unk10 + 0x6000 - (w->unk66 << 8);
        if (w->unk34 > 0) {
            w->unk34 = 0;
        }
        if (w->unk2E > 0) {
            w->unk2E = 0;
        }
        w->unk58 |= 4;
    }
    if (gObjManagerFlags & 0x100) {
        return;
    }
    if (o->unk1 != 0) {
        s16 s = (UnkSin_023526cc(o->unk1 << 8) * 0xaa00) >> 12;
        lim = ((s * ((w->unk10 - o->unk10) >> 7)) >> 8) - 0x1200;
    }
    v = o->unk14 + lim;
    if (w->unk14 + (w->unk67 << 8) > v) {
        w->unk14 = v - (w->unk67 << 8);
    }
}
