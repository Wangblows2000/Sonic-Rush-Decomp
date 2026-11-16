#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x30 - 0x10];
    s16 unk30;
    u8 pad32[0x5C - 0x32];
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x184 - 0x6C];
    UnkStruct_ObjParam *unk184;
} UnkStruct_0231df98;

void func_ov000_0231dee0(void);

void func_ov000_0231df98(UnkStruct_0231df98 *w) {
    if (w->unk68 != func_ov000_0231dee0 || (Rand() & 1))
        StageObj_SetAnimation(w, 0);
    w->unk5C |= 4;
    w->unk68 = func_ov000_0231dee0;
    w->unkC = Rand() & 0x1f;
    switch (w->unk184->unk2) {
    case 3:
    case 8:
    case 13:
        w->unk30 = 0;
        break;
    }
}
