#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x478 - 0x20c];
    s16 unk478[20];
} UnkWork_02318080;

void func_ov000_02317e50(void);

void func_ov000_02318080(UnkWork_02318080 *w) {
    u16 i;

    w->base.unk68 = func_ov000_02317e50;
    w->base.unkC = w->base.unk1E0->unk8 << 8;
    if (w->base.unkC > 0x8000)
        w->base.unkC -= 0x10000;
    for (i = 0; i < 20; i++)
        w->unk478[i] = w->base.unkC;
}
