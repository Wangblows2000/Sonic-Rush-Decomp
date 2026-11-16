#include <nitro/types.h>

typedef struct {
    u16 id;
    u8 pad2[0x12];
} UnkEnt_022cfc60;

typedef struct {
    u8 hdr[0x20];
    UnkEnt_022cfc60 e[1];
} UnkTable_022cfc60;

typedef struct {
    u8 pad0[0x18];
    s32 ofs;
} UnkRes_022cfc60;

s32 func_02039c18(s32 a, u16 id);
void func_02033590(void *a, UnkRes_022cfc60 *res, u16 idx, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);

#pragma thumb on
void func_ov001_022cfc60(void *a, UnkRes_022cfc60 *res, s32 idx) {
    UnkTable_022cfc60 *tbl;
    u16 id;
    s32 v0;
    s32 v1;

    tbl = (UnkTable_022cfc60 *)((u8 *)res + res->ofs);
    id = tbl->e[(u16)idx].id;
    v0 = func_02039c18(0, id);
    v1 = func_02039c18(1, id);
    func_02033590(a, res, idx, 0, 0, 0, v0, 2, 0, 0, v1, 4, 0);
}
#pragma thumb reset
