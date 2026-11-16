#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_020215dc;
typedef struct { u8 unk0; u8 count; u8 unk2[4]; u16 subOffset; } UnkDict_020215dc;
typedef struct { u8 unk0[2]; u16 dictOffset; } UnkBlock_020215dc;
typedef struct { u8 unk0[8]; u32 blockOffset; } UnkStruct_020215dc;

void NNS_G3dReleaseMdlPltt(UnkStruct_020215dc *s) {
    UnkBlock_020215dc *blk = (UnkBlock_020215dc *)((u8 *)s + s->blockOffset);
    UnkDict_020215dc *d = (UnkDict_020215dc *)((u8 *)blk + blk->dictOffset);
    u32 i;
    for (i = 0; i < d->count; i++) {
        UnkSub_020215dc *sub = (UnkSub_020215dc *)((u8 *)d + d->subOffset);
        u8 *e = sub->data + sub->elemSize * i;
        if (e[3] & 1) e[3] &= ~1;
    }
}
