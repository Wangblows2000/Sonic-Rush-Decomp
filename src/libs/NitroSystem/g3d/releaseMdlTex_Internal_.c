#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_0202189c;
typedef struct { u8 unk0[4]; u8 unk4[6]; u16 subOffset; } UnkDict_0202189c;
typedef struct { u8 unk0[0x14]; u32 flags; u8 unk18[0xC]; fx32 unk24; fx32 unk28; } UnkEntry_0202189c;
typedef struct { u16 idxOffset; u8 count; u8 flags; } UnkList_0202189c;

void releaseMdlTex_Internal_(UnkDict_0202189c *d, UnkList_0202189c *l) {
    u8 *idx = (u8 *)d + l->idxOffset;
    u32 i;
    for (i = 0; i < l->count; i++) {
        UnkSub_0202189c *sub = (UnkSub_0202189c *)(d->unk4 + d->subOffset);
        UnkEntry_0202189c *e = (UnkEntry_0202189c *)((u8 *)d + *(u32 *)(sub->data + sub->elemSize * idx[i]));
        e->flags &= 0xC00F0000;
        e->unk24 = 0x1000;
        e->unk28 = e->unk24;
    }
    l->flags &= ~1;
}
