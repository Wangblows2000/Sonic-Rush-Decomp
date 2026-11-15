#include <nitro/types.h>

#pragma thumb on

typedef struct { u16 unk0; u16 unk2; } UnkElem_02048594;
typedef struct { void (*fn)(void); u8 unk4[0xc]; u16 unk10; u16 unk12; UnkElem_02048594 *unk14; s8 unk18; } UnkStruct_02048594;
void func_02048550(void);
void func_02048594(UnkStruct_02048594 *s) { s->unk10 = s->unk14[s->unk18].unk2; s->fn = func_02048550; }

#pragma thumb reset
