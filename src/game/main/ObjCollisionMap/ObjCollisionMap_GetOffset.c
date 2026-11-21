#include <nitro/types.h>

typedef struct { u8 unk0[0x5c]; u16 flags; } UnkSpr;
typedef struct { UnkSpr *spr; u8 unk4[0x18]; s16 unk1C; s16 unk1E; u16 unk20; u16 unk22; } UnkStruct_02067c6c;
void ObjCollisionMap_GetOffset(UnkStruct_02067c6c *s, s16 *x, s16 *y) { *x = s->unk1C; *y = s->unk1E; if (s->spr == NULL) return; if (s->spr->flags & 1) *x = -(s->unk1C + (s->unk20 << 3)); if (s->spr->flags & 2) *y = -(s->unk1E + (s->unk22 << 3)); }
