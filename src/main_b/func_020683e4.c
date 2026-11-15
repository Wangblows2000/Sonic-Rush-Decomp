#include <nitro/types.h>

typedef struct { void *unk0; u16 unk4; } UnkStruct_020683e4;
void func_02039bb0(s32 a, void *p);
void func_020683e4(UnkStruct_020683e4 *s, s32 a) { if (s->unk0 == NULL) return; s->unk4--; if (s->unk4 != 0) return; func_02039bb0(a, s->unk0); s->unk0 = NULL; }
