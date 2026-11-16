#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2e];
    s16 unk2E;
    u8 pad30[0x20c - 0x30];
    u32 unk20C;
    u8 pad210[0x214 - 0x210];
    s16 unk214;
    u8 pad216[2];
    s16 unk218;
    u8 pad21A[0x220 - 0x21a];
    s32 unk220;
    s32 unk224;
    s32 unk228;
    s32 unk22C;
    s32 unk230;
    s32 unk234;
} UnkStruct_022f784c;

void BossFPartD8_Unk_ov000_022f784c(UnkStruct_022f784c *p, s32 a, s32 b, s16 c) {
    p->unk220 = a;
    p->unk224 = 0x30000;
    p->unk228 = 0;
    p->unk22C = b;
    p->unk230 = 0x30000;
    p->unk234 = 0;
    if (p->unk20C & 2) {
        p->unk218 = c;
        p->unk2E = 0;
        return;
    }
    if (a < b)
        p->unk2E = c;
    else
        p->unk2E = -c;
    p->unk214 = p->unk2E;
}
