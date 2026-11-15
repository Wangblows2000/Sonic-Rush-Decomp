#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; } UnkStruct_0204189c;
extern u32 data_02087cb8;
extern u8 data_0224ffa0;
void func_0203dc28(void);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void func_020417ac(void);
void func_0204189c(UnkStruct_0204189c *s) { vu32 z; data_02087cb8 &= ~0x100; data_02087cb8 |= 0x200; func_0203dc28(); z = 0; MIi_CpuClear32(z, &data_0224ffa0, 4); s->unk1A = 0; s->fn = func_020417ac; }
