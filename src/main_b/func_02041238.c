#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; } UnkStruct_02041238;
u32 WH_GetSystemState(void);
void func_020411d8(void);
void func_02041238(UnkStruct_02041238 *s) { if (WH_GetSystemState() == 0) { s->unk1A = 0; s->fn = func_020411d8; } }
