#include <nitro/types.h>

typedef struct { u8 unk0[0x8c]; u32 unk8C; } UnkStruct_02087d04;
extern UnkStruct_02087d04 gDisplaySub;
extern u8 data_02087d90;
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void GX_DisableBankForSubOBJExtPltt(void);
void GX_SetBankForSubOBJExtPltt(s32 a);
void Vram_SetBankForSubOBJExtPltt(s32 mode) { vu32 z = 0; MIi_CpuClear32(z, &data_02087d90, 4); switch (mode) { case 0x100: gDisplaySub.unk8C = 0x68a0000; break; default: GX_DisableBankForSubOBJExtPltt(); return; } GX_SetBankForSubOBJExtPltt(mode); }
