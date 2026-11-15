#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0; u8 unk1; u16 unk2; } UnkElem_020485c4;
typedef struct { void (*fn)(void); u8 unk4[0x10]; UnkElem_020485c4 *unk14; s8 unk18; } UnkStruct_020485c4;
extern s16 data_022c4da0;
void func_02060f40(void);
void func_020485ac(void);
void func_020485c4(UnkStruct_020485c4 *s) { data_022c4da0 = s->unk14[s->unk18].unk1; func_02060f40(); s->fn = func_020485ac; }

#pragma thumb reset
