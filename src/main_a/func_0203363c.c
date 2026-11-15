#include <nitro/types.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);

typedef struct { u32 unk0; u32 unk4; } UnkEntry_0203363c;
typedef struct { u32 unk0; u32 entriesOffset; } UnkHeader_0203363c;
typedef struct { u32 unk0; u32 unk4; u16 unk8; u16 unkA; u32 unkC; UnkHeader_0203363c *header; u8 unk14[8]; u32 unk1C; u8 unk20[0xA]; u16 unk2A; u32 unk2C; u32 unk30; u32 unk34; u32 unk38; u32 unk3C; u32 unk40; u32 unk44; } UnkStruct_0203363c;

void func_0203363c(UnkStruct_0203363c *s, UnkHeader_0203363c *h, u16 idx, u32 d, u32 e, u32 f, u32 g, u32 hh, u32 i) {
    vu16 z = 0;
    MIi_CpuClear16(z, s, sizeof(UnkStruct_0203363c));
    s->header = h;
    s->unk2A = 0x10;
    s->unk2C = d;
    s->unk8 = idx;
    s->unk0 = e;
    s->unk30 = f;
    s->unk34 = g;
    s->unk3C = hh;
    s->unk40 = i;
    s->unk1C = ((UnkEntry_0203363c *)((u8 *)h + h->entriesOffset))[idx].unk4;
}
