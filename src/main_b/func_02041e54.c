#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_022501a0;
extern UnkStruct_022501a0 data_022501a0;
extern u16 data_0224ffa0;
typedef struct { void (*fn)(void); u8 unk4[0x14]; u16 unk18; u16 unk1A; } UnkStruct_02041e54;
void func_02041454(void);
void func_02041e54(UnkStruct_02041e54 *s) { if (data_0224ffa0 == (data_022501a0.unk2 | 1)) { s->unk18 = 0; s->unk1A = 0; s->fn = func_02041454; } }
