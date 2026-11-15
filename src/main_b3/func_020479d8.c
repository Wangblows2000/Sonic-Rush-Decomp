#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 unk2C;
    u8 pad30[0x14];
    u16 unk44;
    u16 unk46;
} UnkSpr_020479d8;
typedef struct { u16 unk0; u16 unk2; u8 pad4[0x10]; } UnkEnt_020479d8;
typedef struct { u8 pad0[0x1c]; UnkEnt_020479d8 ent[1]; } UnkTbl_020479d8;
typedef struct { u8 pad0[0x18]; s32 unk18; } UnkRes_020479d8;
typedef struct {
    u8 pad0[0x310];
    s16 unk310;
    u8 pad312[0x86];
    UnkRes_020479d8 *res; /* 0x398 */
    UnkSpr_020479d8 spr[0x33]; /* 0x39c */
    UnkSpr_020479d8 spr2[0xf]; /* 0x11f4 */
} UnkWork_020479d8;

extern u8 data_02072cb8;
extern u16 *data_02078b68[];
extern u16 data_020732fc[];

void *func_02039c18(u32 a, u32 b);
void func_0203363c(UnkSpr_020479d8 *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
void func_0203344c(UnkSpr_020479d8 *s, u32 a, u32 b);

#pragma thumb on
static inline u32 GetLang_020479d8(void) {
    u32 l;
    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        l = data_02072cb8;
        break;
    default:
        l = 1;
        break;
    }
    return l;
}

void func_020479d8(UnkWork_020479d8 *w) {
    u32 i;
    s32 k = w->unk310;
    u32 lang = data_02072cb8;
    u32 idx;
    u16 *p;
    UnkSpr_020479d8 *spr;

    switch (lang) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        idx = lang;
        break;
    default:
        idx = 1;
        break;
    }
    p = data_02078b68[idx] + k;

    for (i = 0; i < 2; i++, k++, p++) {
        spr = &w->spr2[k];
        func_0203363c(spr, w->res, *p, 0x4010, 0, 0, func_02039c18(0, ((UnkTbl_020479d8 *)((u8 *)w->res + w->res->unk18))->ent[*p].unk2), 0, 0x05000200);
        spr->unk44 = data_020732fc[k];
        func_0203344c(spr, 0, 0);
        w->spr2[k].unk2C |= 0x200;
    }
    switch (lang) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        idx = lang;
        break;
    default:
        idx = 1;
        break;
    }
    p = data_02078b68[idx] + 0xc;
    for (i = 0xc; i < 0xf; i++, p++) {
        spr = &w->spr2[i];
        func_0203363c(spr, w->res, *p, 0x4010, 0, 0, func_02039c18(0, ((UnkTbl_020479d8 *)((u8 *)w->res + w->res->unk18))->ent[*p].unk2), 0, 0x05000200);
        spr->unk44 = data_020732fc[i];
        func_0203344c(spr, 0, 0);
    }
}
#pragma thumb reset
