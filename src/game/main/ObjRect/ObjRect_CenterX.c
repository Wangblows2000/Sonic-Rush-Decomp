#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; s16 unk8; s16 unkA; s16 unkC; s16 unkE; s16 unk10; s16 unk12; } UnkStruct_0206d6f8;
s32 ObjRect_CenterX(UnkStruct_0206d6f8 *s) { return (s->unk0 + ((s->unkC + s->unk10) >> 1)) << 8; }
