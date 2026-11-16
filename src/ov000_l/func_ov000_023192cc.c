#include <ov000_b/stageobj.h>

extern s16 data_ov000_0236a2bc[];

void func_ov000_0231907c(void);
void func_ov000_02318e68(void);

void func_ov000_023192cc(UnkStruct_StageObj *w) {
    UnkStruct_ObjParam *p = w->unk1E0;

    if (p->unk2 == 0xbe) {
        if (data_ov000_0236a2bc[p->unk6] != 0) {
            w->unkC = 0x200;
            if (w->unk5C & 2)
                w->unk14 -= 0x200;
            else
                w->unk14 += 0x200;
        }
        w->unk68 = func_ov000_0231907c;
        return;
    }
    if (data_ov000_0236a2bc[p->unk6] != 0) {
        w->unkC = 0x200;
        if (!(w->unk5C & 1)) {
            w->unk10 += 0x200;
        } else {
            w->unk10 -= 0x200;
            w->unkC = -w->unkC;
        }
    }
    w->unk68 = func_ov000_02318e68;
}
