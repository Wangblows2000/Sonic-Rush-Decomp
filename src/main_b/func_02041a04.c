#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; } UnkStruct_02041a04;
extern u32 gSystemFlags;
u32 WH_GetSystemState(void);
void func_020419e4(void);
void func_02041a04(UnkStruct_02041a04 *s) { if (WH_GetSystemState() != 5) return; gSystemFlags |= 0x100; gSystemFlags &= ~0x200; s->unk1A = 0; s->fn = func_020419e4; }
