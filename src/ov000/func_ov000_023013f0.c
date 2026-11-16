#include <nitro/types.h>

typedef struct UnkStruct_023013f0_2 {
    u8 pad[2];
    u16 unk2;
} UnkStruct_023013f0_2;

typedef struct UnkStruct_023013f0 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x5c - 0x10];
    u16 unk5C;
    u8 pad2[0x150 - 0x5e];
    s32 unk150;
    u8 pad3[0x1e0 - 0x154];
    UnkStruct_023013f0_2 *unk1E0;
    u8 pad4[0x238 - 0x1e4];
    u32 unk238;
    s32 unk23C;
} UnkStruct_023013f0;

void StageObj_SetAnimation(UnkStruct_023013f0 *s, s32 a);

void func_ov000_023013f0(UnkStruct_023013f0 *s) {
    if (s->unk1E0->unk2 != 6)
        return;
    s->unkC--;
    if (s->unkC != 0)
        return;
    if (s->unk238 & 0x1000) {
        s->unkC = 0xb4;
        StageObj_SetAnimation(s, 0);
        s->unk150 = s->unk23C;
    } else {
        s->unkC = 0x5a;
        StageObj_SetAnimation(s, 1);
        s->unk150 = 0;
    }
    s->unk238 ^= 0x1000;
    s->unk5C |= 4;
}
