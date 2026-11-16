#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef struct {
    u8 unk0[8];
    u32 ofsMat;
    u8 unkC[0xc];
    u8 numMat;
} UnkMdl_02025e1c;

typedef struct {
    u16 ofsTex;
    u16 ofsPltt;
    UnkResDict dict;
} UnkMatSet_02025e1c;

typedef struct {
    u8 unk0[0x1e];
    u16 flag;
} UnkMat_02025e1c;

void NNSi_G3dModifyMatFlag(UnkMdl_02025e1c *mdl, BOOL set, u16 mask)
{
    u32 n;
    u32 i;
    UnkMatSet_02025e1c *ms;
    n = mdl->numMat;
    ms = (UnkMatSet_02025e1c *)((u8 *)mdl + mdl->ofsMat);
    for (i = 0; i < n; i++) {
        UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)&ms->dict + ms->dict.ofsEntry);
        UnkMat_02025e1c *mat = (UnkMat_02025e1c *)((u8 *)ms + *(u32 *)(ent->data + ent->sizeUnit * i));
        if (set)
            mat->flag |= mask;
        else
            mat->flag &= ~mask;
    }
}
