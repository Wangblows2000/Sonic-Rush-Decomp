#pragma opt_propagation off
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x2E - 0x14];
    s16 unk2E;
    u8 pad30[0x218 - 0x30];
    s32 unk218;
    u8 pad21C[0x35C - 0x21C];
    s32 unk35C;
} UnkStruct_022f6978;

void BossFPartDE_NarrowEnds(UnkStruct_022f6978 *s) {
    s32 t;

    if (s->unk2E > 0x2000) {
        s->unk35C = s->unk35C + (((0x8000 - (s->unk35C - s->unk10)) << 14) >> 16);
        s->unk218 = s->unk218 - (((0x8000 - (s->unk10 - s->unk218)) << 12) >> 16);
    } else if (s->unk2E < -0x2000) {
        t = s->unk10 - s->unk218;
        s->unk218 -= ((0x8000 - t) << 14) >> 16;
        s->unk35C = s->unk35C + (((0x8000 - (s->unk35C - s->unk10)) << 12) >> 16);
    } else {
        t = s->unk10 - s->unk218;
        s->unk218 -= ((0x8000 - t) << 13) >> 16;
        s->unk35C = s->unk35C + (((0x8000 - (s->unk35C - s->unk10)) << 13) >> 16);
    }
    t = s->unk218;
    if (t < 0) {
        t = 0;
    } else if (t > 0x34000) {
        t = 0x34000;
    }
    s->unk218 = t;
    t = s->unk35C;
    if (t < 0) {
        t = 0;
    } else if (t > 0x34000) {
        t = 0x34000;
    }
    s->unk35C = t;
}
