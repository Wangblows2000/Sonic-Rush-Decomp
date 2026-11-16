#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x238 - 0x20C];
    u32 unk238;
} UnkStruct_02301108_P;

void func_ov000_023010a4(UnkStruct_StageObj *w);

void func_ov000_02301108(UnkStruct_StageObj *w) {
    UnkStruct_02301108_P *p = w->unk88;

    w->unkC--;
    if (w->unkC == 0) {
        if (p != NULL)
            p->unk238 |= 0x100;
        func_ov000_023010a4(w);
        return;
    }
    if (p == NULL)
        return;
    w->unk10 = p->base.unk10;
    w->unk14 = p->base.unk14 + (((0x3c - w->unkC) * 0xf) << 8) / 60;
}
