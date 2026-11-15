#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02000dfc(void *p);
void func_0200148c(void *p);

typedef struct { u32 unk0; u32 unk4; u8 unk8[2]; u8 unkA; u8 unkB[0xD]; fx32 unk18; fx32 unk1C; fx32 unk20; u8 unk24[0x3C]; u8 unk60[0xB4]; fx32 unk114; fx32 unk118[5]; u16 unk12C; u16 unk12E[5]; } UnkStruct_02031c1c;

void func_02031c1c(UnkStruct_02031c1c *s, u32 a) {
    vu32 zero = 0;
    u32 i;
    MIi_CpuClear32(zero, s, sizeof(UnkStruct_02031c1c));
    s->unk0 = 0;
    s->unk4 = a;
    s->unkA = 3;
    s->unk18 = s->unk1C = s->unk20 = 0x1000;
    func_02000dfc(s->unk24);
    func_0200148c(s->unk60);
    for (i = 0; i < 5; i++) {
        s->unk118[i] = 0x1000;
        s->unk12E[i] = 0x1000;
    }
    s->unk114 = 0x1000;
    s->unk12C = 0x1000;
}
