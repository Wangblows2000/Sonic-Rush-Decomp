#include <nitro/types.h>

typedef struct { u8 unk0[0x64]; s8 unk64; s8 unk65; s8 unk66; s8 unk67; } UnkStruct_02068e40;
void StageObj_SetMapHitbox(UnkStruct_02068e40 *s, s8 a, s8 b, s8 c, s8 d) { s->unk64 = a; s->unk65 = b; s->unk66 = c; s->unk67 = d; }
