#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
} UnkStruct_02316db0_88;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x54 - 0x10];
    u32 unk54;
    u8 pad58[4];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_02316db0_88 *unk88;
} UnkStruct_02316db0;

void StageObj_SetAnimation(void *w, s32 a);

void func_ov000_02316db0(UnkStruct_02316db0 *w) {
    UnkStruct_02316db0_88 *p = w->unk88;

    if (p == NULL) {
        return;
    }
    switch (w->unkC) {
    case 0:
        if (p->unkC == 0) {
            return;
        }
        if (w->unk54 & 0x1000) {
            StageObj_SetAnimation(w, 8);
        } else {
            StageObj_SetAnimation(w, 3);
        }
        w->unkC++;
        w->unk5C |= 4;
        break;
    case 1:
        if (p->unk8 != 0) {
            return;
        }
        if (w->unk54 & 0x1000) {
            StageObj_SetAnimation(w, 9);
        } else {
            StageObj_SetAnimation(w, 4);
        }
        w->unkC++;
        w->unk5C |= 4;
        break;
    case 2:
        w->unk8++;
        if (w->unk8 <= 0x10) {
            return;
        }
        if (w->unk54 & 0x1000) {
            StageObj_SetAnimation(w, 7);
        } else {
            StageObj_SetAnimation(w, 2);
        }
        w->unk8 = 0;
        w->unkC = 0;
        w->unk5C |= 4;
        break;
    }
}
