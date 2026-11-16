#include <nitro/types.h>

typedef struct {
    u8 pad0[0x184];
    u8 *unk184;
    s32 unk188;
    u8 pad18C[0x194 - 0x18c];
    u32 unk194;
} UnkWork_023265d8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_023265d8 *work;
} UnkTask_023265d8;

void func_ov000_0232644c(u8 *e);
void StageObj_Destructor(UnkTask_023265d8 *t);

void func_ov000_023265d8(UnkTask_023265d8 *t) {
    UnkWork_023265d8 *w = t->work;
    u8 *e = w->unk184;

    if (e != NULL && e[0] == 0xff && e[1] == 0xff)
        func_ov000_0232644c(e);
    else if (!(w->unk194 & 0x10) && e != NULL)
        e[0] = w->unk188;
    StageObj_Destructor(t);
}
