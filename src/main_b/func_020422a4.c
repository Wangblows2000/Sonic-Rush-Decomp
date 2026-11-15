#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u32 unkC; u8 unk10[0xa]; u16 unk1A; } UnkStruct_020422a4;
extern u8 data_022c43a0;
u32 func_0203e934(void);
void func_0203e9a8(void *a, u16 b);
BOOL func_0203e844(void);
void func_0204138c(void);
void func_02042220(void);
void func_020422a4(UnkStruct_020422a4 *s) { if (func_0203e934() != 1) return; func_0203e9a8(&data_022c43a0, 0x70); if (func_0203e844() == 0) { s->unkC = 1; s->fn = func_0204138c; } else { s->unk1A = 0; s->fn = func_02042220; } }
