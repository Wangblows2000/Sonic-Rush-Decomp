#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_0231fad4;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    UnkVec_0231fad4 unkC;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x3C - 0x32];
    void *unk3C;
    void *unk40;
    void *unk44;
    UnkVec_0231fad4 unk48;
    UnkVec_0231fad4 unk54;
    u8 pad60[0x6C - 0x60];
    s16 unk6C;
} UnkStruct_0231fad4_T;

typedef struct {
    u8 pad0[0x210];
    s32 unk210;
    void *unk214;
    void *unk218;
} UnkStruct_0231fad4_S;

typedef struct {
    u8 pad0[0xDC];
    s32 unkDC;
} UnkStruct_0231fad4_C;

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_0231fad4;

extern UnkStruct_0231fad4_S *gBoss3;
extern UnkStruct_0231fad4_T data_ov000_0236a48c;
extern UnkStruct_0231fad4_C data_ov000_0236a444;

static inline void SetVec(UnkVec_0231fad4 *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void StageZoneScript_Zone3Boss(UnkStruct_0231fad4 *w) {
    UnkStruct_0231fad4_T *t = &data_ov000_0236a48c;
    UnkStruct_0231fad4_S *s = gBoss3;

    if (s->unk210 == 1) {
        if (w->unk4 >= 0x1e) {
            if (w->unk4 == 0x1e) {
                data_ov000_0236a444.unkDC = 1;
                t->unk0 = s->unk218;
                t->unk4 = gBoss3->unk218;
                t->unk8 = gBoss3->unk218;
                SetVec(&t->unkC, 0, 0x20000, 0);
                t->unk30 = 0x200;
                t->unk3C = gBoss3->unk214;
                t->unk40 = NULL;
                t->unk44 = gBoss3->unk214;
                t->unk54.y = -0x200000;
                SetVec(&t->unk48, 0, 0x20000, 0);
                SetVec(&t->unk54, 0, -0x1c0000, 0);
                SetVec(&t->unk48, 0, 0x28000, 0);
                t->unk6C = 0x400;
                w->unk4++;
            }
        } else {
            w->unk4++;
        }
    } else {
        w->unk4 = 0;
    }
}
