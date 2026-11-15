#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_020216e4;
typedef struct { u8 unk0[4]; u8 unk4[6]; u16 subOffset; } UnkDict_020216e4;
typedef struct { u8 unk0[0x1C]; u16 unk1C; } UnkEntry_020216e4;
typedef struct { u16 idxOffset; u8 count; u8 flags; } UnkList_020216e4;
typedef struct { u8 unk0[0x2C]; u32 unk2C; } UnkObj_020216e4;
typedef struct { u16 base; u16 flags; } UnkFrm_020216e4;

void func_020216e4(UnkDict_020216e4 *d, UnkList_020216e4 *l, UnkObj_020216e4 *o, UnkFrm_020216e4 *f) {
    u8 *idx = (u8 *)d + l->idxOffset;
    u16 a = o->unk2C;
    u16 b = f->base;
    u32 i;
    if (!(f->flags & 1)) {
        b = b >> 1;
        a = a >> 1;
    }
    for (i = 0; i < l->count; i++) {
        UnkSub_020216e4 *sub = (UnkSub_020216e4 *)(d->unk4 + d->subOffset);
        UnkEntry_020216e4 *e = (UnkEntry_020216e4 *)((u8 *)d + *(u32 *)(sub->data + sub->elemSize * idx[i]));
        e->unk1C = b + a;
    }
    l->flags |= 1;
}
