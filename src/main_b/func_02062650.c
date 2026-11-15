#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x7a]; u16 unk7E; } UnkStruct_02062650;
void func_020625fc(void);
void func_02062650(UnkStruct_02062650 *s) { s->unk7E = 0; s->fn = func_020625fc; }

#pragma thumb reset
