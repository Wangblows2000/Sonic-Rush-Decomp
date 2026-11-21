#include <nitro/types.h>
#include "NitroSystem/g3drs.h"

typedef struct {
    u8 unk0[0xc];
    u16 ofsBlk;
} UnkRes_02026274;

typedef struct {
    u8 unk0[8];
    UnkResDict dict;
} UnkBlk_02026274;

void *NNS_G3dGetAnmByIdx(UnkRes_02026274 *res, u32 idx)
{
    UnkBlk_02026274 *blk = (UnkBlk_02026274 *)((u8 *)res + *(u32 *)((u8 *)res + res->ofsBlk));
    UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)&blk->dict + blk->dict.ofsEntry);
    u32 *p = (u32 *)(ent->data + ent->sizeUnit * idx);
    if (p != NULL)
        return (u8 *)blk + *p;
    return NULL;
}
