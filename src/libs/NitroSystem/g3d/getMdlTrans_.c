#include <nitro/types.h>
#include "NitroSystem/g3drs.h"

typedef struct {
    u32 flag;
    u8 unk4[0x48];
    fx32 scale[3];
} UnkJnt_02027840;

extern UnkRS *data_02084f9c;

void getMdlTrans_(UnkJnt_02027840 *jnt)
{
    UnkResDict *dict = data_02084f9c->unkD4;
    UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)dict + dict->ofsEntry);
    u8 *data = (u8 *)dict + *(u32 *)(ent->data + ent->sizeUnit * data_02084f9c->c[1]);
    fx32 *p;
    if (*(u16 *)data & 1) {
        jnt->flag |= 4;
        return;
    }
    p = (fx32 *)(data + 4);
    jnt->scale[0] = p[0];
    jnt->scale[1] = p[1];
    jnt->scale[2] = p[2];
}
