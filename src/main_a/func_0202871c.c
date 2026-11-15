#include <nitro/types.h>

typedef struct { fx32 m[4][3]; fx32 unk30; fx32 unk34; } UnkMtx_0202871c;
typedef struct { u8 unk0[0x18]; fx32 unk18; fx32 unk1C; u8 unk20[0xE]; u16 unk2E; } UnkStruct_0202871c;

void func_0202871c(UnkMtx_0202871c *m, UnkStruct_0202871c *s) {
    m->m[0][0] = s->unk18;
    m->m[1][2] = s->unk1C;
    m->m[0][1] = 0;
    m->unk30 = 0;
    m->unk34 = (s->unk2E * (s->unk1C * -2 + 0x2000)) << 3;
    m->m[1][1] = 0;
}
