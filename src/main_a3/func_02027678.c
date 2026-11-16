#include <nitro/types.h>
#include "main_a2/g3drs.h"

typedef struct {
    u32 flag;
    u8 unk4[0x24];
    fx32 rot[9];
} UnkJnt_02027678;

typedef struct {
    u8 a;
    u8 b;
    u8 c;
    u8 d;
} UnkPivot_02027678;

extern UnkRS *data_02084f9c;
extern UnkPivot_02027678 data_02072b44[];

void MI_Zero36B(void *p);

void func_02027678(UnkJnt_02027678 *jnt)
{
    fx32 a;
    fx32 b;
    int piv;
    UnkResDict *dict = data_02084f9c->unkD4;
    UnkResDictEntry *ent = (UnkResDictEntry *)((u8 *)dict + dict->ofsEntry);
    u8 *data = (u8 *)dict + *(u32 *)(ent->data + ent->sizeUnit * data_02084f9c->c[1]);
    u16 flag = *(u16 *)data;
    s16 *p = (s16 *)(data + 4);
    if (!(flag & 1))
        p += 6;
    if (!(flag & 2)) {
        if (flag & 8) {
            piv = (flag & 0xf0) >> 4;
            a = p[0];
            b = p[1];
            MI_Zero36B(jnt->rot);
            jnt->rot[piv] = (*(u16 *)data & 0x100) ? -0x1000 : 0x1000;
            jnt->rot[data_02072b44[piv].a] = a;
            jnt->rot[data_02072b44[piv].b] = b;
            if (*(u16 *)data & 0x200)
                b = -b;
            jnt->rot[data_02072b44[piv].c] = b;
            if (*(u16 *)data & 0x400)
                a = -a;
            jnt->rot[data_02072b44[piv].d] = a;
        } else {
            jnt->rot[0] = *(s16 *)(data + 2);
            jnt->rot[1] = p[0];
            jnt->rot[2] = p[1];
            jnt->rot[3] = p[2];
            jnt->rot[4] = p[3];
            jnt->rot[5] = p[4];
            jnt->rot[6] = p[5];
            jnt->rot[7] = p[6];
            jnt->rot[8] = p[7];
        }
    } else {
        jnt->flag |= 2;
    }
}
