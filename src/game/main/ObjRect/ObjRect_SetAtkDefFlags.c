#include <nitro/types.h>

typedef struct { u8 unk0[0xA]; u16 flags; u8 unkC[0x14]; s32 unk20; s32 unk24; } UnkStruct_0206e2b4;

/* Matched (CW 1.2): two separate read-modify-writes of `flags` are required. */
void ObjRect_SetAtkDefFlags(UnkStruct_0206e2b4 *s, s32 a, s32 b) {
    s->unk20 = a;
    s->unk24 = b;
    s->flags &= ~0x200;
    s->flags &= ~0x400;
}
