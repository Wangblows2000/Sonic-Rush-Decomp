#include <nitro/types.h>

typedef struct {
    u16 id;
    u8 pad2[0x12];
} UnkEnt_022cfd34;

typedef struct {
    u8 hdr[0x20];
    UnkEnt_022cfd34 e[1];
} UnkTable_022cfd34;

typedef struct {
    u8 pad0[0x18];
    s32 ofs;
} UnkRes_022cfd34;

s32 ObjVram_Alloc(s32 a, u16 id);
void AnimatorSprite_Init(void *s, void *res, u16 idx, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

#pragma thumb on
void StaffRoll_InitSpriteMainById(void *s, UnkRes_022cfd34 *res, s32 idx) {
    UnkTable_022cfd34 *tbl;

    tbl = (UnkTable_022cfd34 *)((u8 *)res + res->ofs);
    AnimatorSprite_Init(s, res, idx, 0, 0, 0, ObjVram_Alloc(0, tbl->e[(u16)idx].id), 0, 0x5000200);
}
#pragma thumb reset
