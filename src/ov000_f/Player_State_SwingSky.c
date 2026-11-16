#include <nitro/types.h>

typedef struct UnkStruct_0234e0f8 UnkStruct_0234e0f8;

struct UnkStruct_0234e0f8 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 x;
    s32 y;
    s32 z;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x30 - 0x2C];
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u8 pad5C[0x32C - 0x5C];
    u32 unk32C;
    u32 unk330;
    void (*unk334)(UnkStruct_0234e0f8 *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_0234e0f8 *w);
    u8 pad344[0x3E8 - 0x344];
    UnkStruct_0234e0f8 *unk3E8;
    u8 pad3EC[0x3EE - 0x3EC];
    s16 unk3EE;
    u8 pad3F0[0x422 - 0x3F0];
    u16 unk422;
};

extern s16 data_0206e470[];

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IDX_0234e0f8(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_0234e0f8(a) data_0206e470[(u32)(IDX_0234e0f8(a) << 1)]

void func_ov000_0234a064(UnkStruct_0234e0f8 *w, u8 ang);

void Player_State_SwingSky(UnkStruct_0234e0f8 *w) {
    u8 ang;
    s32 vy;

    if (w->unk3E8 != NULL) {
        w->unk1C = w->x;
        w->unk20 = w->y;
        w->unk24 = w->z;
        w->x = w->unk3E8->unk1C;
        w->y = w->unk3E8->unk20;
        w->unk1 = w->unk3E8->unk1 - 0x40;
        w->unk28 = w->x - w->unk1C;
        w->unk2A = w->y - w->unk20;
        w->unk34 = w->unk3E8->unk28;
        w->unk30 = w->unk3E8->unk2A;
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
    ang = w->unk1 + 0x40;
    vy = w->unk30 >> 1;
    w->unk3E8 = NULL;
    w->unk32C &= ~0x2000;
    w->unk58 &= ~0x2000;
    w->unk32C &= ~0x100000;
    w->unk1 = 0;
    w->unk34 = -(w->unk34 << 1);
    w->unk3EE = 0;
    w->unk330 &= ~0x20;
    if (w->unk422 & 3) {
        w->unk340(w);
        w->unk30 -= (s16)ABS((vy * SIN_0234e0f8((u16)(ang << 8))) >> 12);
        return;
    }
    func_ov000_0234a064(w, 0);
    w->unk334(w);
}
