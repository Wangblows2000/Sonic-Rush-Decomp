#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef struct {
    u8 unk0[8];
    u32 ofsMat;
} UnkMdl_02025cd4;

typedef struct {
    u16 ofsTex;
    u16 ofsPltt;
    UnkResDict dict;
} UnkMatSet_02025cd4;

typedef struct {
    u8 unk0[8];
    u32 flag;
} UnkMat_02025cd4;

void NNS_G3dMdlSetMdlEmi(UnkMdl_02025cd4 *mdl, u32 idx, u32 val)
{
    UnkMatSet_02025cd4 *ms = (UnkMatSet_02025cd4 *)((u8 *)mdl + mdl->ofsMat);
    UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)&ms->dict + ms->dict.ofsEntry);
    UnkMat_02025cd4 *mat = (UnkMat_02025cd4 *)((u8 *)ms + *(u32 *)(ent->data + ent->sizeUnit * idx));
    mat->flag = (mat->flag & 0x8000ffff) | (val << 16);
}
