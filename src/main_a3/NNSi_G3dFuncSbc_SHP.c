#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef void (*UnkShpFn_02024400)(UnkRS *rs, u32 opt, void *shp, u32 idx);

extern UnkShpFn_02024400 data_02077b80[];

void NNSi_G3dFuncSbc_SHP(UnkRS *rs, u32 opt)
{
    if (!(rs->flag & 0x200) && (rs->flag & 1) && !(rs->flag & 2)) {
        u32 idx = rs->c[1];
        UnkResDict *dict = rs->resShp;
        UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)dict + dict->ofsEntry);
        u8 *shp = (u8 *)dict + *(u32 *)(ent->data + ent->sizeUnit * idx);
        data_02077b80[*(u16 *)shp](rs, opt, shp, idx);
    }
    rs->c += 2;
}
