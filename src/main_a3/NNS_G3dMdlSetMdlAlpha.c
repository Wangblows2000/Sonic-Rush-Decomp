#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef struct {
    u8 unk0[8];
    u32 ofsMat;
} UnkMdl_02025c8c;

typedef struct {
    u16 ofsTex;
    u16 ofsPltt;
    UnkResDict dict;
} UnkMatSet_02025c8c;

typedef struct {
    u8 unk0[0xc];
    u32 flag;
} UnkMat_02025c8c;

void NNS_G3dMdlSetMdlAlpha(UnkMdl_02025c8c *mdl, u32 idx, u32 val)
{
    UnkMatSet_02025c8c *ms = (UnkMatSet_02025c8c *)((u8 *)mdl + mdl->ofsMat);
    UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)&ms->dict + ms->dict.ofsEntry);
    UnkMat_02025c8c *mat = (UnkMat_02025c8c *)((u8 *)ms + *(u32 *)(ent->data + ent->sizeUnit * idx));
    mat->flag = (mat->flag & ~0x1f0000) | (val << 16);
}
