#include <nitro/types.h>

typedef struct { fx32 m[4][3]; fx32 unk30; fx32 unk34; } UnkMtx_020296dc;
typedef struct { u8 unk0[0x24]; fx32 unk24; fx32 unk28; u16 unk2C; u16 unk2E; } UnkStruct_020296dc;

void texmtxCalc_flagRS_3dsmax(UnkMtx_020296dc *m, UnkStruct_020296dc *s) {
    m->m[0][0] = 0x1000;
    m->m[1][2] = 0x1000;
    m->m[0][1] = 0;
    m->unk30 = (-s->unk24 * s->unk2C) << 4;
    m->unk34 = (s->unk28 * s->unk2E) << 4;
    m->m[1][1] = 0;
}
