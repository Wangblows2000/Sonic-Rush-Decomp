#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0xe]; u16 unk12; u8 unk14[0x1af4]; void *unk1B08; u8 unk1B0C[0x1ca]; u8 unk1CD6[0x1e]; } UnkStruct_020411d8;
u16 func_02042774(s32 a);
void func_0203bb94(void *p, u16 v);
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void func_02040f40(void);
void func_020411d8(UnkStruct_020411d8 *s) { vu16 z; s->unk12 = func_02042774(0x18); func_0203bb94(s->unk1B08, s->unk12); z = 0; MIi_CpuClear16(z, s->unk1CD6, 0x1e); s->fn = func_02040f40; }
