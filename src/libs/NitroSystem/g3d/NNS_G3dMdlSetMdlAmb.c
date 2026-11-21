#include <nitro/types.h>
#include "NitroSystem/g3drs.h"

typedef struct {
    u8 unk0[8];
    u32 ofsMat;
} UnkMdl_02025d1c;

typedef struct {
    u16 ofsTex;
    u16 ofsPltt;
    UnkResDict dict;
} UnkMatSet_02025d1c;

typedef struct {
    u8 unk0[4];
    u32 flag;
} UnkMat_02025d1c;

void NNS_G3dMdlSetMdlAmb(UnkMdl_02025d1c *mdl, u32 idx, u32 val)
{
    UnkMatSet_02025d1c *ms = (UnkMatSet_02025d1c *)((u8 *)mdl + mdl->ofsMat);
    UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)&ms->dict + ms->dict.ofsEntry);
    UnkMat_02025d1c *mat = (UnkMat_02025d1c *)((u8 *)ms + *(u32 *)(ent->data + ent->sizeUnit * idx));
    mat->flag = (mat->flag & 0x8000ffff) | (val << 16);
}
