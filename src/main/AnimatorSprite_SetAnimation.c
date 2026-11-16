#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; } UnkEntry_02033550;
typedef struct { u32 unk0; u32 entriesOffset; } UnkHeader_02033550;
typedef struct {
    u8 unk0[8];
    u16 index;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    UnkHeader_02033550 *header;
    u8 unk14[8];
    u32 unk1C;
    u8 unk20[8];
    u16 unk28;
    u16 unk2A;
    u32 unk2C;
} UnkStruct_02033550;

/* Selects entry `index` (8-byte records at header + entriesOffset) and resets
 * counters. Matched (CW 1.2). Called from 206 sites. */
void AnimatorSprite_SetAnimation(UnkStruct_02033550 *s, u16 index) {
    s->index = index;
    s->unkA = 0;
    s->unk28 = 0;
    s->unkE = 0;
    s->unkC = 0;
    s->unk2C &= ~3;
    s->unk1C = ((UnkEntry_02033550 *)((u8 *)s->header + s->header->entriesOffset))[index].unk4;
}
