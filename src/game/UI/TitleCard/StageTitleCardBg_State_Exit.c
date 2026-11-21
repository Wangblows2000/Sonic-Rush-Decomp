#include <nitro/types.h>

typedef struct { void (*fn)(void); s32 unk4; u8 unk8[4]; s16 unkC; } UnkObj_0205c624;
void StageTitleCardBg_State_Done(void);
void StageTitleCardBg_State_Exit(void *a, UnkObj_0205c624 *o) { o->unkC += 0x28; if (o->unkC >= 0x1000) { o->unkC = 0x1000; o->fn = StageTitleCardBg_State_Done; } o->unk4 = (s32)(((s64)o->unkC * 0x80000 + 0x800) >> 12); }
