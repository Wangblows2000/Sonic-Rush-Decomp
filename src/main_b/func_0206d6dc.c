#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; s16 unk8; s16 unkA; s16 unkC; s16 unkE; s16 unk10; s16 unk12; } UnkStruct_0206d6dc;
s32 func_0206d6dc(UnkStruct_0206d6dc *s) { return (s->unk4 + ((s->unkE + s->unk12) >> 1)) << 8; }
