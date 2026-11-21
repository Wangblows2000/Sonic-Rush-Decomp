#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
    s32 x;
    s32 y;
    s32 z;
    u8 pad54[0x138 - 0x54];
} UnkObj_0233d8e8;

typedef struct {
    u8 pad0[0x20C];
    u32 unk20C;
    u8 pad210[0x238 - 0x210];
    s32 unk238;
    u8 pad23C[0x37C - 0x23C];
    UnkObj_0233d8e8 obj[5];
    s32 unk994;
    s32 unk998;
    u8 pad99C[0x9A0 - 0x99C];
    s32 unk9A0;
    s32 unk9A4;
    s32 unk9A8;
    s32 unk9AC;
    s32 unk9B0;
    s32 unk9B4;
    s32 unk9B8;
    s32 unk9BC;
    s32 unk9C0;
} UnkStruct_0233d8e8;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_0233d8e8_4560;

extern UnkStruct_0233d8e8 *gBossF;
extern u32 gBossStatus;
extern UnkStruct_0233d8e8_4560 gGameState;

void Model_Draw(UnkObj_0233d8e8 *o);

void ZoneFx_ZoneFBoss_Update(void) {
    UnkStruct_0233d8e8 *w = gBossF;
    UnkObj_0233d8e8 *o;

    if (w == NULL) {
        return;
    }
    if (!(gBossStatus & 1)) {
        return;
    }
    if (w->unk238 != 0) {
        if (gGameState.unk20 > 0x1680 && w->unk998 != 0) {
            w->unk9AC += 0x64000;
            w->unk9B0 += 0x64000;
            if (w->unk9AC >= 0x1088000) {
                w->unk9AC = w->unk9B0;
                w->unk9B0 = w->unk9AC - 0x1388000;
                w->unk9B4 = w->unk9B8;
                if (gGameState.unk20 > 0x1680) {
                    w->unk9B8 = 1;
                }
            }
            o = w->unk9B4 ? &w->obj[3] : &w->obj[2];
            o->x = 0x1a0000;
            o->z = 0;
            o->y = w->unk9AC;
            Model_Draw(o);
            o = w->unk9B8 ? &w->obj[3] : &w->obj[2];
            o->x = 0x1a0000;
            o->z = 0;
            o->y = w->unk9B0;
            Model_Draw(o);
        } else if (gGameState.unk20 > 0xb40 && w->unk994 != 0) {
            w->unk9A4 += 0xc000;
            w->unk9A8 += 0xc000;
            o = &w->obj[1];
            w->unk20C |= 0x10;
            if (w->unk9A4 >= 0x1088000) {
                w->unk9A4 = w->unk9A8;
                if (gGameState.unk20 > 0x1680) {
                    if (w->unk9A4 >= 0x1088000) {
                        w->unk998 = 1;
                    }
                } else {
                    w->unk9A8 = w->unk9A4 - 0x1388000;
                }
            }
            o->x = 0x1a0000;
            o->z = 0;
            o->y = w->unk9A4;
            Model_Draw(o);
            o->y = w->unk9A8;
            Model_Draw(o);
        } else {
            o = &w->obj[0];
            o->y += 0x1800;
            if (o->y >= 0xc8000) {
                if (gGameState.unk20 > 0xb40) {
                    w->unk994 = 1;
                } else {
                    if (w->unk9A0 != 0) {
                        w->unk9A0 = 0;
                        o->x = 0xc8000;
                        o->z = -0x3e8000;
                    } else {
                        w->unk9A0 = 1;
                        o->x = 0x258000;
                        o->z = -0x708000;
                    }
                    o->y = -0x3c8000;
                }
            }
            Model_Draw(o);
        }
    }
    w->unk9BC += 0x10000;
    w->unk9C0 += 0x10000;
    o = &w->obj[4];
    if (w->unk9BC >= 0x1088000) {
        w->unk9BC = w->unk9C0;
        w->unk9C0 = w->unk9BC - 0x1388000;
    }
    o->x = 0x1a0000;
    o->z = 0;
    o->y = w->unk9BC;
    Model_Draw(o);
    o->y = w->unk9C0;
    Model_Draw(o);
}
