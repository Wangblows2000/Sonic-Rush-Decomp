#include <nitro/types.h>

typedef struct {
    u8 a;
    u8 b;
    u8 c;
    u8 pad;
} UnkRec_022ec38c;

typedef struct {
    u8 pad0[0x28];
    s32 count;
    u8 pad2c[0xc];
    u8 a;
    u8 b;
    u8 c;
    u8 pad3b;
    UnkRec_022ec38c recs[10];
} UnkStruct_022ec38c;

typedef struct {
    u8 pad0[0x1f1];
    u8 a;
    u8 b;
    u8 c;
    u8 pad1f4[0xc];
} UnkSave_022ec38c;

typedef struct {
    u8 pad0[0x10];
    u8 a;
    u8 b;
    u8 c;
} UnkCfg_022ec38c;

extern UnkCfg_022ec38c data_022c4840;
extern u8 data_022c4680[];

#pragma thumb on
void func_ov001_022ec38c(UnkStruct_022ec38c *p) {
    s32 i;
    s32 n;

    p->a = data_022c4840.a;
    p->b = data_022c4840.b;
    p->c = data_022c4840.c;
    n = 0;
    for (i = 0; i < 10; i++) {
        p->recs[i].a = data_022c4680[i * 0x20 + 0x1f1];
        p->recs[i].b = data_022c4680[i * 0x20 + 0x1f2];
        p->recs[i].c = data_022c4680[i * 0x20 + 0x1f3];
        if ((p->recs[i].a != 0 || p->recs[i].b != 0 || p->recs[i].c != 0) && i == n) {
            n++;
        }
    }
    p->count = n;
}
#pragma thumb reset
