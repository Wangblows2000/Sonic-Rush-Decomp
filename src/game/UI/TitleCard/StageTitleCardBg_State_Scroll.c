#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205c6ac;
typedef struct { void (*fn)(void); u32 unk4; s32 unk8; } UnkObj_0205c6ac;
void StageTitleCardBg_State_BeginExit(void);
void StageTitleCardBg_State_Scroll(UnkStruct_0205c6ac *s, UnkObj_0205c6ac *o) { o->unk8 += 0x1000; if (s->flags & 0x10000000) o->fn = StageTitleCardBg_State_BeginExit; }
