#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x24];
    s32 unk24;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern s16 data_0206e470[];

static inline s32 FX_Mul(s32 a, s32 b) { return (s32)(((s64)a * b + 0x800) >> 12); }

void GmkLand_Unk_ov000_0230ef70(UnkStruct_StageObj *s) {
    UnkStruct_ObjParam *prm = s->unk1E0;
    u16 f = prm->unk4;
    u8 k = f & 7;
    u16 c = s->unkC;
    s32 bx = s->unk1C;
    s32 by = s->unk20;
    s16 rx = (prm->unk8 * 0x800) >> 12;
    s16 ry = (prm->unk9 * 0x800) >> 12;
    u16 ang;
    s32 x;
    s32 y;

    if (k)
        ang = (gGameState.unk24 * k + c) & 0x3ff;
    else
        ang = ((gGameState.unk24 + (c >> 2)) & 0xff) << 2;
    if (f & 0x20) {
        x = (bx + ((rx * data_0206e470[(((u16)((ang << 6) + 0x8000) & 0xffff) >> 4) * 2]) >> 12)) << 8;
        y = (by + ((ry * data_0206e470[(((u16)(ang << 6) & 0xffff) >> 4) * 2]) >> 12)) << 8;
    } else {
        s16 sn = data_0206e470[(((u16)(ang << 6) & 0xffff) >> 4) * 2];
        x = (bx + ((rx * sn) >> 12)) << 8;
        y = (by + ((ry * sn) >> 12)) << 8;
    }
    s->unk28 = x - s->unk10;
    s->unk2A = y - s->unk14;
    s->unk10 = x;
    s->unk14 = y;
}
