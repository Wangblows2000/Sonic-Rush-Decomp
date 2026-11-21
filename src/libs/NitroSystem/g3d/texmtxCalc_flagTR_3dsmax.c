#include <nitro/types.h>

typedef struct { fx32 m[4][3]; fx32 unk30; fx32 unk34; } UnkMtx_020294a4;
typedef struct { u8 unk0[0x18]; fx32 unk18; fx32 unk1C; u8 unk20[0xC]; u16 unk2C; u16 unk2E; } UnkStruct_020294a4;

void texmtxCalc_flagTR_3dsmax(UnkMtx_020294a4 *m, UnkStruct_020294a4 *s) {
    m->m[0][0] = s->unk18;
    m->m[1][2] = s->unk1C;
    m->m[0][1] = 0;
    m->unk30 = ((0x1000 - s->unk18) * s->unk2C) << 3;
    m->unk34 = ((0x1000 - s->unk1C) * s->unk2E) << 3;
    m->m[1][1] = 0;
}
