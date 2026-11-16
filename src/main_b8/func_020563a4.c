#include <nitro/types.h>

typedef struct {
    s32 x, y, z;
} UnkVec_020563a4;

typedef struct {
    u8 pad0[0x48];
    UnkVec_020563a4 pos;
    u8 pad54[0xe4];
} UnkModel_020563a4;

typedef struct {
    u8 pad0[0x20];
    s8 count;
    s8 cur;
    u8 pad22[0x1a];
    UnkModel_020563a4 models[15];
    u8 pad[0x1310 - 0x3c - 15 * 0x138];
    UnkVec_020563a4 pos;
} UnkWork_020563a4;

void func_020316f4(UnkModel_020563a4 *m);
void func_020315e0(UnkModel_020563a4 *m);

#pragma thumb on
void func_020563a4(UnkWork_020563a4 *w) {
    UnkModel_020563a4 *m;

    if (w->cur < 0)
        return;
    m = &w->models[w->cur];
    m->pos = w->pos;
    func_020316f4(m);
    func_020315e0(m);
}
#pragma thumb reset
