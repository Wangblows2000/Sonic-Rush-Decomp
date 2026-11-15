#include <nitro/types.h>

typedef struct { u8 unk0[0x8c]; u32 unk8C; } UnkStruct_02087d04;
extern UnkStruct_02087d04 data_02087d04;
extern u8 data_02087d90;
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void func_02002e28(void);
void func_02003178(s32 a);
void func_0203a18c(s32 mode) { vu32 z = 0; MIi_CpuClear32(z, &data_02087d90, 4); switch (mode) { case 0x100: data_02087d04.unk8C = 0x68a0000; break; default: func_02002e28(); return; } func_02003178(mode); }
