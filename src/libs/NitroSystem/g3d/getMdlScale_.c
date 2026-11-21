#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef void (*UnkJntCb_020277d8)(void *jnt, const s16 *p, const u8 *cmd, u32 flag);

extern UnkRS *data_02084f9c;

void getMdlScale_(void *jnt)
{
    UnkResDict *dict = data_02084f9c->unkD4;
    UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)dict + dict->ofsEntry);
    u8 *data = (u8 *)dict + *(u32 *)(ent->data + ent->sizeUnit * data_02084f9c->c[1]);
    u16 flag = *(u16 *)data;
    s16 *p = (s16 *)(data + 4);
    if (!(flag & 1))
        p += 6;
    if (!(flag & 2)) {
        if (flag & 8)
            p += 2;
        else
            p += 8;
    }
    ((UnkJntCb_020277d8)data_02084f9c->unkE8)(jnt, p, data_02084f9c->c, flag);
}
