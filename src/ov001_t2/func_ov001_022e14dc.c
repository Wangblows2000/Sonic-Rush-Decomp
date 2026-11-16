#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x14];
    u16 unk44;
} UnkSpr_022e14dc;

typedef struct {
    u8 pad0[0x14];
} UnkEntPad_022e14dc;

typedef struct {
    u8 pad0[0x1e];
    u16 id;
} UnkEnt_022e14dc;

typedef struct { u16 unk0; u16 unk2; u8 pad4[0x10]; } UnkEnt2_022e14dc;
typedef struct { u8 pad0[0x1c]; UnkEnt2_022e14dc ent[1]; } UnkTbl_022e14dc;

typedef struct {
    u8 pad0[0x18];
    s32 offset;
} UnkRes_022e14dc;

void *func_02039c18(u32 a, u32 b);
void func_0203363c(UnkSpr_022e14dc *s, UnkRes_022e14dc *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, void *h);

#pragma thumb on
void func_ov001_022e14dc(UnkSpr_022e14dc *s, UnkRes_022e14dc *res, u32 idx, u16 v, s32 mode) {
    UnkTbl_022e14dc *t = (UnkTbl_022e14dc *)((u8 *)res + res->offset);
    func_0203363c(s, res, idx, 0x40, 0, 0, func_02039c18(0, t->ent[idx].unk2), 0, (void *)0x5000200);
    s->flags &= ~0x60000;
    s->flags |= mode << 17;
    s->unk44 = v;
}
#pragma thumb reset
