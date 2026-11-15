#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; u16 unk4; u16 unk6; } UnkHdr;
typedef struct { u8 unk0[0x2c]; u32 unk2C; u8 unk30[4]; u16 unk34; } UnkStruct_02038018;
u32 func_02038018(UnkStruct_02038018 *s, s32 i) { u8 *base = (u8 *)s + s->unk34; u8 *t = base + ((UnkHdr *)base)->unk6; u16 stride = *(u16 *)t; return ((s->unk2C & 0xffff) + *(u16 *)(t + stride * i + 4)) << 3; }
