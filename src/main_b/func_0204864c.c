#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0; u8 unk1; u16 unk2; } UnkElem_0204864c;
typedef struct { void (*fn)(void); u8 unk4[0x10]; UnkElem_0204864c *unk14; s8 unk18; } UnkStruct_0204864c;
extern s16 data_022c4678;
void func_02057364(void);
void func_02048634(void);
void func_0204864c(UnkStruct_0204864c *s) { data_022c4678 = s->unk14[s->unk18].unk1; func_02057364(); s->fn = func_02048634; }

#pragma thumb reset
