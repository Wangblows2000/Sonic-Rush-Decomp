#include <nitro/types.h>

typedef struct { u8 unk0[8]; s32 off8; s32 offC; s32 off10; u32 flags; u16 unk18; u16 unk1A; } UnkHdr_020526bc;
typedef struct { u32 flags; u8 unk4[0x28]; u16 unk2C; u16 unk2E; void *unk30; void *unk34; void *unk38; u16 unk3C; u16 unk3E; } UnkStruct_020526bc;
void func_020526bc(UnkStruct_020526bc *s, UnkHdr_020526bc *h) { s->flags &= ~0x1000; if (h->flags & 0x10) s->flags |= 0x1000; s->unk30 = (u8 *)h + h->off10; s->unk34 = (u8 *)h + h->off8; s->unk38 = (u8 *)h + h->offC; s->unk3C = h->unk18; s->unk3E = h->unk1A; s->unk2C = h->unk18; s->unk2E = h->unk1A; }
