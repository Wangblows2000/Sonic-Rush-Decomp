#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    u8 unk8;
    u8 pad9;
    u8 unkA;
    u8 padB;
    s32 unkC;
} UnkStruct_02315e84_W;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02315e84_W unk10;
} UnkStruct_02315e84_B;

typedef struct {
    u8 pad0[0x58];
    u32 unk58;
    u8 pad5C[0x68 - 0x5C];
    void *unk68;
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
} UnkStruct_02315e84_P;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x44 - 0x10];
    s16 unk44;
    s16 unk46;
    u8 pad48[0x54 - 0x48];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_02315e84_P *unk88;
    u8 pad8C[0x9A - 0x8C];
    s16 unk9A;
    u16 unk9C;
    u8 pad9E[0xE2 - 0x9E];
    s16 unkE2;
} UnkStruct_02315e84;

extern UnkStruct_02315e84_B *gDisplayTable[];

s16 ObjShift_SetFromRef(s16 v, s32 a, s32 b, s32 c, s32 d, s32 e);
void StageObj_SetAnimation(void *w, s32 a);
void func_ov000_0235020c(void);

void func_ov000_02315e84(UnkStruct_02315e84 *w) {
    UnkStruct_02315e84_P *p;
    u16 i;
    UnkStruct_02315e84_W *s;

    switch (w->unk8) {
    case 0:
        w->unk44 = ObjShift_SetFromRef(w->unk44, 0x100, 8, 2, 0, 8);
        w->unk46 = ObjShift_SetFromRef(w->unk46, 0x100, 0x180, 4, 0, 8);
        w->unk5C &= ~0x20;
        if (w->unk46 == 0x100 && w->unk44 == 0x100)
            w->unk8 = 1;
        break;
    case 2:
        w->unk44 = ObjShift_SetFromRef(w->unk44, 8, 0x100, 2, 0, 8);
        w->unk46 = ObjShift_SetFromRef(w->unk46, 0x180, 0x100, 4, 0, 8);
        if (w->unk46 == 0x180 && w->unk44 == 8) {
            w->unk5C |= 0x20;
            w->unk54 |= 4;
            return;
        }
        break;
    }
    p = w->unk88;
    if (p == NULL)
        return;
    if ((p->unk32C & 0x80) && (p->unk58 & 0x2000) && w->unk8 == 1) {
        if (w->unk9C == 0) {
            StageObj_SetAnimation(w, 1);
            w->unk5C |= 4;
        }
        w->unkC = 0;
        return;
    }
    if (w->unk9C != 0) {
        StageObj_SetAnimation(w, 0);
        w->unk5C |= 4;
    }
    if (w->unk8 == 0)
        return;
    if (p->unk68 == func_ov000_0235020c) {
        if (w->unkC == 0) {
            for (i = 0; i < 2; i++) {
                s = &gDisplayTable[i]->unk10;
                s->unk1 = 0;
                s->unk0 = 0xff;
                if (w->unk5C & 2) {
                    if (i != 0)
                        s->unk5 = w->unkE2 - 0x10;
                    else
                        s->unk5 = w->unk9A - 0x10;
                    s->unk4 = 0xff;
                } else {
                    s->unk5 = 0;
                    if (i != 0)
                        s->unk4 = w->unkE2 + 0x10;
                    else
                        s->unk4 = w->unk9A + 0x10;
                }
                s->unkA = 0xff;
                s->unk8 = 0xfe;
                s->unkC = 1;
            }
        }
        w->unkC++;
        if (w->unkC > 0x24)
            w->unk8 = 2;
    } else {
        w->unk8 = 2;
    }
}
