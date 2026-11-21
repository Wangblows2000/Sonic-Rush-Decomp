#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0xc]; s16 unk10; } UnkStruct_02059f00;
void SaveMsg_State_Format(void);
void SaveMsg_State_FormatDelay(UnkStruct_02059f00 *s) { s->unk10--; if (s->unk10 <= 0) s->fn = SaveMsg_State_Format; }
