#include <nitro/types.h>

typedef struct { u8 pad0[0x10]; s32 unk10; } UnkPal_02047b78;
typedef struct {
    u8 pad0[0x10];
    UnkPal_02047b78 *pal; /* 0x10 */
    u8 pad14[0x18];
    u32 unk2C;
    u8 pad30[0x14];
    u16 unk44;
    u16 unk46;
} UnkSpr_02047b78;
typedef struct { u16 unk0; u16 unk2; u8 pad4[0x10]; } UnkEnt_02047b78;
typedef struct { u8 pad0[0x1c]; UnkEnt_02047b78 ent[1]; } UnkTbl_02047b78;
typedef struct { u8 pad0[0x18]; s32 unk18; } UnkRes_02047b78;
typedef struct {
    u8 pad0[0x394];
    UnkRes_02047b78 *res; /* 0x394 */
    u8 pad398[4];
    UnkSpr_02047b78 spr[0x33]; /* 0x39c */
} UnkWork_02047b78;

typedef struct UnkNode_02047b78 UnkNode_02047b78;
extern UnkNode_02047b78 *data_02250438;
extern UnkNode_02047b78 *data_02250434;
extern u16 data_02073404[];
typedef struct { s32 unk0; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;

void *func_02039c18(u32 a, u32 b);
void func_0203363c(UnkSpr_02047b78 *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
void func_0203344c(UnkSpr_02047b78 *s, u32 a, u32 b);
void func_020331f8(UnkSpr_02047b78 *s);
void func_020356bc(void *p, u32 a, u32 b);
void func_02035754(void);

#pragma thumb on
static inline void LoadPalA_02047b78(u8 *src, u32 a, u32 b) {
    UnkNode_02047b78 *s0 = data_02250438;
    UnkNode_02047b78 *s1 = data_02250434;
    data_02250438 = NULL;
    data_02250434 = NULL;
    func_020356bc(src + 0xcc, a, b);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
}

static inline void LoadPalB_02047b78(u8 *src, u32 a, u32 b) {
    UnkNode_02047b78 *s0 = data_02250438;
    UnkNode_02047b78 *s1 = data_02250434;
    data_02250438 = NULL;
    data_02250434 = NULL;
    func_020356bc(src + 4, a, b);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
}

static inline UnkEnt_02047b78 *GetEnt_02047b78(UnkRes_02047b78 *h, u16 idx) {
    UnkTbl_02047b78 *t = (UnkTbl_02047b78 *)((u8 *)h + h->unk18);
    return &t->ent[idx];
}

void func_02047b78(UnkWork_02047b78 *w) {
    u32 i;
    UnkSpr_02047b78 *spr;

    for (i = 0; i < 0x33; i++) {
        spr = &w->spr[i];
        func_0203363c(spr, w->res, (u16)i, 0x4010, 0, 0, func_02039c18(0, ((UnkTbl_02047b78 *)((u8 *)w->res + w->res->unk18))->ent[(u16)i].unk2), 0, 0x05000200);
        spr->unk44 = data_02073404[i];
        func_0203344c(spr, 0, 0);
    }
    w->spr[19].unk2C &= ~0x10;
    w->spr[29].unk2C &= ~0x10;
    w->spr[35].unk2C &= ~0x10;
    w->spr[40].unk2C &= ~0x10;
    func_020331f8(&w->spr[19]);
    func_020331f8(&w->spr[29]);
    func_020331f8(&w->spr[35]);
    func_020331f8(&w->spr[40]);
    if (data_022c4560.unk0 == 1) {
        LoadPalA_02047b78((u8 *)spr->pal + spr->pal->unk10, *(u32 *)((u8 *)w + 0x3d8), *(u32 *)((u8 *)w + 0x3dc) + *(u16 *)((u8 *)w + 0x3e0) * 32);
    } else {
        LoadPalB_02047b78((u8 *)spr->pal + spr->pal->unk10, *(u32 *)((u8 *)w + 0x3d8), *(u32 *)((u8 *)w + 0x3dc) + *(u16 *)((u8 *)w + 0x3e0) * 32);
    }
    for (i = 0x23; i <= 0x26; i++)
        w->spr[i].unk2C |= 0x200;
}
#pragma thumb reset
