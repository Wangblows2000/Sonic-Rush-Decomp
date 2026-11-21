#include <nitro/types.h>

typedef struct { u8 unk0[0x48c]; u32 flags2; } UnkStruct_0205ca64;
typedef struct UnkObj_0205ca64 { void (*fn)(UnkStruct_0205ca64 *, struct UnkObj_0205ca64 *); u8 unk4[8]; u16 unkC; } UnkObj_0205ca64;
void StageTitleCardPlayer_State_Zoom(UnkStruct_0205ca64 *a, UnkObj_0205ca64 *o);
void StageTitleCardPlayer_State_BeginZoom(UnkStruct_0205ca64 *s, UnkObj_0205ca64 *o) { s->flags2 |= 0x400; o->unkC = 0; o->fn = StageTitleCardPlayer_State_Zoom; o->fn(s, o); }
