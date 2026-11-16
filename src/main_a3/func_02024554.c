#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef void (*UnkMatFn_02024554)(UnkRS *rs, u32 opt, void *mat, u32 idx);

typedef struct {
    u16 ofsTex;
    u16 ofsPltt;
    UnkResDict dict;
} UnkMatSet_02024554;

extern UnkMatFn_02024554 data_02077b70[];

void func_02024554(UnkRS *rs, u32 opt)
{
    u32 flag = rs->flag;
    if (!(flag & 0x200)) {
        u32 idx = rs->c[1];
        if ((flag & 1) || !(flag & 8) || idx != rs->curMat) {
            UnkMatSet_02024554 *ms = rs->resMat;
            UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)&ms->dict + ms->dict.ofsEntry);
            u8 *mat = (u8 *)ms + *(u32 *)(ent->data + ent->sizeUnit * idx);
            data_02077b70[*(u16 *)mat](rs, opt, mat, idx);
        }
    }
    rs->c += 2;
}
