#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 limit; } UnkLimits_02040ec0;
typedef struct {
    u8 unk0[0x10];
    u32 flags;
    u32 value;
    u32 unk18;
    u32 unk1C;
    s32 unk20;
    s32 unk24;
    UnkLimits_02040ec0 *limits;
} UnkStruct_02040ec0;

/* Matched (CW 1.2, Thumb). */
void func_02040ec0(UnkStruct_02040ec0 *s, u32 value) {
    if (value < s->limits->limit) s->value = value;
    s->flags &= ~0x49;
    s->unk18 = 0;
    s->unk1C = 0x1000;
    s->unk20 = -1;
    s->unk24 = -1;
    s->flags |= 0x20;
}

#pragma thumb reset
