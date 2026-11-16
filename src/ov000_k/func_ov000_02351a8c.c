#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x28 - 0x18];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x58 - 0x2C];
    u32 unk58;
    u16 unk5C;
} UnkStruct_02351a8c_Obj;

typedef struct UnkStruct_02351a8c UnkStruct_02351a8c;

struct UnkStruct_02351a8c {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 x;
    s32 y;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x2E - 0x24];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    u16 unk62;
    u8 pad64[0x32C - 0x64];
    u32 unk32C;
    u32 unk330;
    void (*unk334)(UnkStruct_02351a8c *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_02351a8c *w);
    u8 pad344[0x3E8 - 0x344];
    UnkStruct_02351a8c_Obj *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    u8 pad3F4[0x3F8 - 0x3F4];
    s32 unk3F8;
    s32 unk3FC;
    u8 pad400[0x420 - 0x400];
    u16 unk420;
    u16 unk422;
};

extern s16 data_0206e470[];

#define IDX_02351a8c(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02351a8c(a) data_0206e470[(u32)(IDX_02351a8c(a) << 1)]
#define COS_02351a8c(a) data_0206e470[(u32)((IDX_02351a8c(a) << 1) + 1)]

s32 func_02068df8(s32 v, s32 a, s32 max);
s32 func_02068db4(s32 v, s32 a);
void func_ov000_0234a064(UnkStruct_02351a8c *w, u8 a);

void func_ov000_02351a8c(UnkStruct_02351a8c *w) {
    u16 m1;
    u16 m2;
    u16 t;
    s32 i;
    s32 r;

    if (w->unk3E8 != NULL) {
        m1 = 0xa0;
        m2 = 0x50;
        if (!(w->unk3E8->unk5C & 1)) {
            m1 ^= m2;
            m2 ^= m1;
            m1 ^= m2;
        }
        w->unk1C = w->x;
        w->unk20 = w->y;
        i = IDX_02351a8c((u16)(w->unk1 << 8));
        r = w->unk3F0;
        w->x = w->unk3E8->x + (s16)((r * data_0206e470[(u32)((i << 1) + 1)]) >> 12);
        w->y = w->unk3E8->y + (s16)((r * data_0206e470[(u32)(i << 1)]) >> 12);
        if (w->unk62 == 0) {
            w->unk62 = 0x40;
        }
        if (w->unk420 & m1) {
            w->unk3FC = (s16)func_02068df8(w->unk3FC, 0x10, 0x180);
        } else if (w->unk3FC > 0) {
            w->unk3FC = (s16)func_02068db4(w->unk3FC, 8);
        }
        if (w->unk420 & m2) {
            w->unk3FC = (s16)func_02068df8(w->unk3FC, -0x10, 0x180);
        } else if (w->unk3FC < 0) {
            w->unk3FC = (s16)func_02068db4(w->unk3FC, 8);
        }
        if (w->unk5C & 1) {
            w->unk3F8 -= w->unk3FC;
        } else {
            w->unk3F8 += w->unk3FC;
        }
        w->unk1 = w->unk3F8 >> 8;
        if (w->unk3E8->unk58 & 0x2000) {
            w->unk2E = w->unk3E8->unk28;
            w->unk30 = w->unk3E8->unk2A;
            w->unk3E8 = NULL;
        }
    }
    if (w->unk3E8 != NULL && !(w->unk422 & 3)) {
        return;
    }
    if (w->unk340 == NULL) {
        return;
    }
    if (w->unk334 == NULL) {
        return;
    }
    w->unk3E8 = NULL;
    w->unk58 |= 0xc0;
    w->unk58 &= ~0x2000;
    w->unk58 &= ~0x4000;
    w->unk32C &= ~0x100000;
    w->unk330 &= ~0x20000;
    w->unk1 = 0;
    if (w->unk422 & 3) {
        w->unk340(w);
        return;
    }
    func_ov000_0234a064(w, 0);
    w->unk334(w);
}
