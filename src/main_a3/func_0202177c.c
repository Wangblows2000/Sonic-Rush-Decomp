#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef struct {
    u8 unk0[8];
    u32 ofsBlk;
} UnkRes_0202177c;

void func_0202189c(void *blk, void *data);

void func_0202177c(UnkRes_0202177c *res)
{
    u8 *blk = (u8 *)res + res->ofsBlk;
    UnkResDict *dict = (UnkResDict *)(blk + *(u16 *)blk);
    u32 i;
    for (i = 0; i < dict->numEntry; i++) {
        UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)dict + dict->ofsEntry);
        u8 *data = ent->data + ent->sizeUnit * i;
        if (data[3] & 1)
            func_0202189c(blk, data);
    }
}
