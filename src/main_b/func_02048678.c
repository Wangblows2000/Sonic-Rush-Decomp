#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0; u8 unk1; u16 unk2; } UnkElem_02048678;
typedef struct { void (*fn)(void); u8 unk4[0xc]; s16 unk10; u16 unk12; UnkElem_02048678 *unk14; s8 unk18; } UnkStruct_02048678;
extern void (*data_020735c4[])(UnkStruct_02048678 *);
void func_02048678(UnkStruct_02048678 *s) { s->unk10--; if (s->unk10 <= 0) data_020735c4[s->unk14[s->unk18].unk0](s); }

#pragma thumb reset
