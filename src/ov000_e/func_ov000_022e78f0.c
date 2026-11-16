#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022e78f0 {
    UnkStruct_StageObj base;
    u8 pad20C[0x42E - 0x20C];
    u16 unk42E;
    u8 pad430[0x440 - 0x430];
    void (*unk440)(struct UnkStruct_022e78f0 *);
    u8 pad444[0x468 - 0x444];
    u16 unk468;
} UnkStruct_022e78f0;

extern UnkStruct_StageObj *data_ov000_0236ac60;
extern u16 data_ov000_02363b44[];
extern u16 data_ov000_02363b6c[];

UnkStruct_StageObj *func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022e7894(UnkStruct_022e78f0 *w);

void func_ov000_022e78f0(UnkStruct_022e78f0 *w) {
    UnkStruct_StageObj *o;

    w->unk468 = data_ov000_02363b44[w->unk42E];
    if (w->base.unk10 < data_ov000_0236ac60->unk10)
        w->base.unk5C &= ~1;
    else
        w->base.unk5C |= 1;
    o = func_ov000_022fad3c(0xd6, data_ov000_0236ac60->unk10, 0x20000, 0, 0, 0, 0, 0, 0);
    o->unkC = data_ov000_02363b6c[w->unk42E];
    w->unk440 = func_ov000_022e7894;
    w->unk440(w);
}
