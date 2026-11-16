#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x43C - 0x20C];
    void *unk43C;
    u8 pad440[0x450 - 0x440];
    u16 unk450;
    u16 unk452;
    u8 pad454[2];
    s16 unk456;
    u16 unk458;
} UnkStruct_022ea900;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

extern UnkStruct_StageObj *gPlayers;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern u8 data_ov000_02363c84[][8], data_ov000_02363ca4[][8], data_ov000_02363cc4[][8];
extern u8 data_ov000_02363ce4[][8], data_ov000_02363d04[][8], data_ov000_02363d24[][8];
extern u8 data_ov000_02363d44[][8], data_ov000_02363d64[][8], data_ov000_02363d84[][8];

void func_ov000_022ea25c(void);
void func_ov000_022ea120(void);
void func_ov000_022e9f3c(void);

void func_ov000_022ea900(UnkStruct_022ea900 *w) {
    s32 d = w->base.unk10 - gPlayers->unk10;
    u16 hp;
    u8 k;

    if (d < 0)
        d = -d;
    hp = data_ov000_02369ac0.unk8;
    if (hp >= 6) {
        if (d < 0x5000)
            k = data_ov000_02363c84[w->unk452][w->unk450];
        else if (d < 0xa000)
            k = data_ov000_02363ca4[w->unk452][w->unk450];
        else
            k = data_ov000_02363cc4[w->unk452][w->unk450];
    } else if (hp >= 4) {
        if (d < 0x5000)
            k = data_ov000_02363ce4[w->unk452][w->unk450];
        else if (d < 0xa000)
            k = data_ov000_02363d04[w->unk452][w->unk450];
        else
            k = data_ov000_02363d24[w->unk452][w->unk450];
    } else {
        if (d < 0x5000)
            k = data_ov000_02363d44[w->unk452][w->unk450];
        else if (d < 0xa000)
            k = data_ov000_02363d64[w->unk452][w->unk450];
        else
            k = data_ov000_02363d84[w->unk452][w->unk450];
    }
    switch (k) {
    case 0:
        break;
    case 1:
        w->unk456 = (Rand() & 0x1f) + 0x14;
        if (w->base.unk10 > gPlayers->unk10)
            w->unk456 = -w->unk456;
        w->unk458 = (Rand() & 0x3f) + 0x1e;
        w->unk43C = func_ov000_022ea25c;
        break;
    case 2:
        w->unk456 = Rand() & 0x1f;
        w->unk43C = func_ov000_022ea120;
        break;
    case 3:
        w->unk43C = func_ov000_022e9f3c;
        break;
    }
    w->unk450++;
    if (w->unk450 >= 8) {
        w->unk450 = 0;
        w->unk452 = Rand() & 3;
    }
}
