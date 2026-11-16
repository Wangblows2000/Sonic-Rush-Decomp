#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0xc]; s16 unk10; } UnkStruct_02059e08;
void SaveMsg_State_WaitStart(void);
void SaveMsg_State_FormatDoneDelay(UnkStruct_02059e08 *s) { s->unk10--; if (s->unk10 <= 0) s->fn = SaveMsg_State_WaitStart; }
