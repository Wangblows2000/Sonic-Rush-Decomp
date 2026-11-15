#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02000dfc(void *p);
void func_0200148c(void *p);

typedef struct { u32 unk0; u32 unk4; u8 unk8[2]; u8 unkA; u8 unkB[0xD]; fx32 unk18; fx32 unk1C; fx32 unk20; u8 unk24[0x3C]; u8 unk60[0x3C]; u32 unk9C; } UnkStruct_020319f0;

void func_020319f0(UnkStruct_020319f0 *s, u32 a) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, s, sizeof(UnkStruct_020319f0));
    s->unk0 = 1;
    s->unk4 = a;
    s->unkA = 3;
    s->unk18 = s->unk1C = s->unk20 = 0x1000;
    func_02000dfc(s->unk24);
    func_0200148c(s->unk60);
    s->unk9C = 1;
}
