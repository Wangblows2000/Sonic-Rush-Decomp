#include <ov000_c/stageobj.h>

void func_02069818(void *w, s32 a);
void func_ov000_023024b0(void);

void func_ov000_02302694(UnkStruct_StageObj *w) {
    switch (w->unk1E0->unk2) {
    case 0xe8:
        break;
    case 0xe9:
        func_02069818(w, 2);
        w->unk30 = -0x101 - (Rand() & 0x7e);
        break;
    case 0xea:
        func_02069818(w, 3);
        w->unk30 = -0x131 - (Rand() & 0x7e);
        break;
    }
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk68 = func_ov000_023024b0;
}
