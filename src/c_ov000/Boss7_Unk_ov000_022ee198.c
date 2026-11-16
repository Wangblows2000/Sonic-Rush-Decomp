#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
    u8 pad118[0x138 - 0x118];
    void *unk138;
    void *unk13C;
    u8 pad140[0x14C - 0x140];
    void *unk14C;
} UnkStruct_Mdl;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2D0 - 0x20C];
    s32 unk2D0;
    s32 unk2D4;
    u8 pad2D8[0x382 - 0x2D8];
    u16 unk382;
    u8 pad384[0x3AE - 0x384];
    u16 unk3AE;
    u8 pad3B0[0x3D0 - 0x3B0];
    s32 unk3D0;
    s32 unk3D4;
    u8 pad3D8[0x4CC - 0x3D8];
    void *unk4CC;
    void *unk4D0;
} UnkStruct_Player;

extern u8 data_ov000_02368c44[];
extern u8 data_ov000_02368bdc[];
extern u8 data_ov000_02368cac[];
extern u8 data_ov000_02368d14[];

void Model_SetResource(UnkStruct_Mdl *m, void *a, u32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void AnimatorSprite_SetAnimation(UnkStruct_Spr *a, s32 b);

void Boss7_Unk_ov000_022ee198(UnkStruct_Player *s, s32 st) {
    s32 prev;
    UnkStruct_Mdl *m;
    s32 a;
    s32 b;

    prev = s->unk3D4;
    s->unk3D4 = st;
    if (s->unk3D0 == 1) {
        if (st >= 0x30)
            st += s->unk3D0 * 0x1b;
        if (prev >= 0x30)
            prev += s->unk3D0 * 0x1b;
    }
    s->base.unk5C &= ~8;
    s->base.unk5C &= ~4;
    s->unk382 &= ~4;
    s->unk3AE &= ~4;
    m = s->base.unk124;
    if (m != NULL) {
        if (data_ov000_02368c44[prev] != data_ov000_02368c44[st]) {
            a = s->unk2D0;
            b = s->unk2D4;
            Model_SetResource(m, m->unk138, data_ov000_02368c44[st]);
            s->base.unk5C &= ~0x400;
            s->unk2D0 = a;
            s->unk2D4 = b;
        }
        if (s->base.unk5C & 0x400)
            ((UnkStruct_Mdl *)s->base.unk124)->unk10C |= 0x40;
        else
            ((UnkStruct_Mdl *)s->base.unk124)->unk10C &= ~0x40;
        if ((st >= 0x48 && st <= 0x4a) || (st >= 0x63 && st <= 0x65)) {
            Model_SetAnimation(s->base.unk124, 0, s->unk4CC, data_ov000_02368bdc[st], 0);
            if (data_ov000_02368cac[st] != 0)
                Model_SetAnimation(s->base.unk124, 4, s->unk4D0, data_ov000_02368cac[st], 0);
        } else {
            Model_SetAnimation(s->base.unk124, 0, ((UnkStruct_Mdl *)s->base.unk124)->unk13C, data_ov000_02368bdc[st], 0);
            if (data_ov000_02368cac[st] != 0)
                Model_SetAnimation(s->base.unk124, 4, ((UnkStruct_Mdl *)s->base.unk124)->unk14C, data_ov000_02368cac[st], 0);
        }
        ((UnkStruct_Mdl *)s->base.unk124)->unk114 = 0x1000;
        s->base.unk5C &= ~0x400;
    }
    s->base.spr0.unk2A = 0x10;
    s->base.spr1.unk2A = 0x10;
    AnimatorSprite_SetAnimation(&s->base.spr0, data_ov000_02368d14[st]);
    AnimatorSprite_SetAnimation(&s->base.spr1, data_ov000_02368d14[st]);
}
