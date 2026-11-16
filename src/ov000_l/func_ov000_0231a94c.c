#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr spr[2];
    u8 pad29C[0x2a4 - 0x29c];
    s32 unk2A4;
} UnkWork_0231a94c;

extern u32 data_ov000_02364494[][3];

void func_ov000_0231a94c(UnkWork_0231a94c *w) {
    u8 i;
    UnkStruct_Spr *s;

    for (i = 0; i < 2; i++) {
        s = &w->spr[i];
        func_02033550(s, (u16)data_ov000_02364494[w->unk2A4][0]);
        s->unk2C |= 4;
    }
}
