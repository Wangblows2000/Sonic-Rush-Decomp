#include <ov000_b/stageobj.h>

typedef struct {
    s32 x;
    s32 y;
} UnkPos_0231a824;

typedef struct {
    u8 pad0[4];
    UnkPos_0231a824 pos;
    u8 padC[0x64 - 0xc];
} UnkEntry_0231a824;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr spr[2];
    u8 pad29C[0x2a4 - 0x29c];
    s32 unk2A4;
} UnkWork_0231a824;

extern s16 data_ov000_02364444[];
extern s16 data_ov000_02364446[];
extern UnkEntry_0231a824 data_ov000_0236aad8[];

void func_0203344c(UnkStruct_Spr *s, s32 a, s32 b);
void func_02032d2c(UnkStruct_Spr *s);

BOOL func_ov000_0231a824(UnkWork_0231a824 *w) {
    UnkStruct_Spr *s;
    s32 px;
    s32 py;
    s16 ox;
    s16 oy;
    u8 i;
    UnkPos_0231a824 *p;

    px = w->base.unk10 >> 8;
    py = w->base.unk14 >> 8;
    ox = data_ov000_02364444[w->unk2A4 * 2];
    oy = data_ov000_02364446[w->unk2A4 * 2];

    for (i = 0; i < 2; i++) {
        p = &data_ov000_0236aad8[i].pos;
        s = &w->spr[i];
        s->unk4 = ox + (px - (p->x >> 8));
        s->unk6 = oy + (py - (p->y >> 8));
        func_0203344c(s, 0, 0);
        func_02032d2c(s);
    }
    return (s->unk2C & 2) != 0;
}
