#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x58 - 0x10];
    u32 unk58;
} UnkStruct_Link;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x29C - 0x20C];
    UnkStruct_Spr spr2;
    UnkStruct_Spr spr3;
} UnkStruct_023043d0;

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void Cannon_RotateBarrel(UnkStruct_023043d0 *w);

void Cannon_State_Aim(UnkStruct_023043d0 *w) {
    UnkStruct_Link *l = w->base.unk204;

    if (l != NULL) {
        if (w->base.unk1F4 & 1) {
            Cannon_RotateBarrel(w);
            w->base.unk1 = (u32)w->base.unk8 >> 8;
        } else if (l->unkC == 0) {
            w->base.unk1F4 |= 1;
            w->base.unk62 = 0x80;
            w->spr2.unk2C &= ~0x60000;
            w->spr3.unk2C &= ~0x60000;
            w->spr2.unk2C |= 0x20000;
            w->spr3.unk2C |= 0x20000;
            w->base.spr0.unk2C &= ~0x60000;
            w->base.spr1.unk2C &= ~0x60000;
            w->base.spr0.unk2C |= 0x20000;
            w->base.spr1.unk2C |= 0x20000;
        }
        if (!(((UnkStruct_Link *)w->base.unk204)->unk58 & 0x2000)) {
            w->base.unk1F4 &= ~3;
            w->base.unk204 = NULL;
            w->base.unkC = 8;
        }
        return;
    }
    if (w->base.unkC == 0)
        return;
    w->base.unkC--;
    w->base.unk6 = -((w->base.unkC * 8 * COS_IDX((s32)((u16)(w->base.unk1 << 8) & 0xffff) >> 4)) >> 12);
    w->base.unk7 = -((w->base.unkC * 8 * SIN_IDX((s32)((u16)(w->base.unk1 << 8) & 0xffff) >> 4)) >> 12);
    if (w->base.unkC == 0) {
        w->spr2.unk2C &= ~0x60000;
        w->spr3.unk2C &= ~0x60000;
        w->spr2.unk2C |= 0x40000;
        w->spr3.unk2C |= 0x40000;
        w->base.spr0.unk2C &= ~0x60000;
        w->base.spr1.unk2C &= ~0x60000;
        w->base.spr0.unk2C |= 0x40000;
        w->base.spr1.unk2C |= 0x40000;
    }
}
