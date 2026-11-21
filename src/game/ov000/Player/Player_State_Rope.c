#include <nitro/types.h>

typedef struct UnkStruct_02354980 UnkStruct_02354980;

struct UnkStruct_02354980 {
    u8 pad0;
    u8 unk1;
    u8 pad2[6 - 2];
    s8 unk6;
    s8 unk7;
    s32 unk8;
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x28 - 0x18];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x2E - 0x2C];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u8 pad5C[0x32C - 0x5C];
    u32 unk32C;
    u8 pad330[0x334 - 0x330];
    void (*unk334)(UnkStruct_02354980 *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_02354980 *w);
    u8 pad344[0x3E8 - 0x344];
    void *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
    u8 pad400[0x422 - 0x400];
    u16 unk422;
};

void Player_Action_SpringLaunch(UnkStruct_02354980 *w, s16 a, s16 b);
void Player_SetSourceMapObject(UnkStruct_02354980 *w, void *p);

void Player_State_Rope(UnkStruct_02354980 *w) {
    void *p;
    s32 dx;
    s32 dy;

    if (w->unk8 != 0) {
        w->unk8--;
        w->unk2E += w->unk3F0;
        w->unk30 += w->unk3F4;
        if (w->unk8 != 0) {
            return;
        }
        p = w->unk3E8;
        w->unk1 = 0;
        w->unk3E8 = NULL;
        w->unk58 |= 0xc0;
        w->unk58 &= ~0x4000;
        w->unk32C &= ~0x100000;
        Player_Action_SpringLaunch(w, w->unk2E, w->unk30);
        Player_SetSourceMapObject(w, p);
        return;
    }
    if (w->unkC != 0) {
        w->unk6 = (((w->unkC << 1) + 8) & 7) - 4;
        w->unk7 = (((w->unkC << 1) + 8) & 7) - 4;
        w->unkC--;
        if (w->unkC == 0) {
            w->unk3F0 = (s16)-(w->unk3F4 >> 20);
            w->unk3F4 = (s16)-((s16)w->unk3F4 >> 4);
            w->unk8 = 0x1c;
            return;
        }
    } else {
        if (w->unk3E8 != NULL) {
            dy = (w->unk3FC - w->y) >> 8;
            dx = (w->unk3F8 - w->x) >> 8;
            if (dx * dx + dy * dy > w->unk3F0) {
                w->unkC = 8;
                w->unk2E = 0;
                w->unk30 = 0;
            }
        }
        if (w->unk28 == 0 && w->unk2A == 0 && w->unk334 != NULL) {
            w->unk1 = 0;
            w->unk3E8 = NULL;
            w->unk58 |= 0xc0;
            w->unk58 &= ~0x8000;
            w->unk58 &= ~0x4000;
            w->unk32C &= ~0x100000;
            w->unk334(w);
            return;
        }
    }
    if (!(w->unk422 & 3)) {
        return;
    }
    if (w->unk340 == NULL) {
        return;
    }
    w->unk1 = 0;
    w->unk3E8 = NULL;
    w->unk58 |= 0xc0;
    w->unk58 &= ~0x4000;
    w->unk32C &= ~0x100000;
    w->unk340(w);
}
