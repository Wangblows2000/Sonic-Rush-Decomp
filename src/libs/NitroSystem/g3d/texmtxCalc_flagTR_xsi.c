#include <nitro/types.h>

typedef struct { fx32 m[4][3]; fx32 unk30; fx32 unk34; } UnkMtx_02029b70;
typedef struct { u8 unk0[0x18]; fx32 unk18; fx32 unk1C; u8 unk20[0xE]; u16 unk2E; } UnkStruct_02029b70;

void texmtxCalc_flagTR_xsi(UnkMtx_02029b70 *m, UnkStruct_02029b70 *s) {
    m->m[0][0] = s->unk18;
    m->m[1][2] = s->unk1C;
    m->m[0][1] = 0;
    m->unk30 = 0;
    m->unk34 = (-s->unk2E * (s->unk1C - 0x1000)) << 4;
    m->m[1][1] = 0;
}
