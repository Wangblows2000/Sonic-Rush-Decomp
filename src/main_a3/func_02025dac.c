#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef struct {
    u8 unk0[8];
    u32 ofsMat;
    u8 unkC[0xc];
    u8 numMat;
} UnkMdl_02025dac;

typedef struct {
    u16 ofsTex;
    u16 ofsPltt;
    UnkResDict dict;
} UnkMatSet_02025dac;

typedef struct {
    u8 unk0[0x10];
    u32 flag;
} UnkMat_02025dac;

void func_02025dac(UnkMdl_02025dac *mdl, BOOL set, u32 mask)
{
    u32 n;
    u32 i;
    UnkMatSet_02025dac *ms;
    n = mdl->numMat;
    ms = (UnkMatSet_02025dac *)((u8 *)mdl + mdl->ofsMat);
    for (i = 0; i < n; i++) {
        UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)&ms->dict + ms->dict.ofsEntry);
        UnkMat_02025dac *mat = (UnkMat_02025dac *)((u8 *)ms + *(u32 *)(ent->data + ent->sizeUnit * i));
        if (set)
            mat->flag |= mask;
        else
            mat->flag &= ~mask;
    }
}
