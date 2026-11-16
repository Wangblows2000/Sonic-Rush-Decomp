#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1f8];
    u16 unk1F8;
} UnkView_023109f4;

void func_ov000_0231096c(void);

void func_ov000_023109f4(UnkStruct_StageObj *w) {
    func_02069818(w, ((UnkView_023109f4 *)w)->unk1F8);
    w->unk68 = func_ov000_0231096c;
    w->unkC = 0;
    w->unk8 = 0;
    w->unk54 |= 2;
    w->unk5C &= ~0x20;
    w->unk5C &= ~0x10;
    if (((UnkView_023109f4 *)w)->unk1F8 == 1)
        w->unkC = -3;
    else
        w->unkC = 3;
}
