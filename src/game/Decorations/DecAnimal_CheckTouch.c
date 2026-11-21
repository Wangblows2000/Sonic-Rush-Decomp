#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x134 - 0x64];
    u8 unk134[0x184 - 0x134];
    UnkStruct_ObjParam *unk184;
    u8 pad188[0x198 - 0x188];
    s16 unk198;
} UnkStruct_0231d904;

s32 StageObj_IsTouched(void *w, void *a);
void DecAnimal_Action_Leave(UnkStruct_0231d904 *w);

void DecAnimal_CheckTouch(UnkStruct_0231d904 *w) {
    if (!(w->unk58 & 1)) {
        if (w->unk184->unk2 != 6 && w->unk184->unk2 != 0xc)
            return;
        if (w->unk30 < -0x80)
            return;
    }
    if (StageObj_IsTouched(w, w->unk134) == 0)
        return;
    if (w->unk198 > 10 || !(Rand() & (0xfff >> w->unk198))) {
        DecAnimal_Action_Leave(w);
        return;
    }
    StageObj_SetAnimation(w, 0);
    w->unk5C |= 4;
    w->unk30 = -0x200;
    w->unk58 |= 0x10;
    switch (w->unk184->unk2) {
    case 0xc:
        if (Rand() & 1)
            w->unk5C &= ~1;
        else
            w->unk5C |= 1;
        w->unk2E = 0x220;
        if (w->unk5C & 1)
            w->unk2E = -w->unk2E;
    case 6:
    case 10:
        StageObj_SetAnimation(w, 1);
        w->unk5C |= 4;
        break;
    }
    if (w->unk198 <= 4 && (Rand() & (0x1f >> w->unk198))) {
    } else {
        if (w->unk198 <= 4)
            w->unk198 = 5;
        switch (w->unk184->unk2) {
        default:
            w->unk30 >>= 1;
            break;
        case 2:
        case 4:
        case 0xe:
            w->unk30 = 0;
            w->unk62 = 0x22;
            break;
        case 10:
        case 0x87:
            break;
        }
        StageObj_SetAnimation(w, 1);
        w->unk5C |= 4;
    }
    w->unk198++;
}
