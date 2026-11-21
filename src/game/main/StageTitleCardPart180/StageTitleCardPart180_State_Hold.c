#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205c140;
typedef struct { void (*fn)(void); u8 unk4[0x48]; s32 unk4C; } UnkObj_0205c140;
void StageTitleCardPart180_State_BeginExit(void);
void StageTitleCardPart180_State_Hold(UnkStruct_0205c140 *s, UnkObj_0205c140 *o) { o->unk4C += 0x400; if (s->flags & 0x10000000) o->fn = StageTitleCardPart180_State_BeginExit; }
