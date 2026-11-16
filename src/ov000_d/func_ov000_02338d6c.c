#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} UnkStruct_02338d6c_Ent;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x20 - 0xC];
    s16 unk20;
    u8 pad22[0x3A - 0x22];
    s8 unk3A;
    u8 pad3B[0x50 - 0x3B];
    s32 unk50;
    u8 pad54[4];
    s32 unk58;
    u8 pad5C[0xC8 - 0x5C];
    u32 unkC8;
} UnkStruct_02338d6c_Cam;

extern UnkStruct_02338d6c_Cam gCamera;
extern UnkStruct_02338d6c_Ent *gPlayers[];

BOOL func_ov000_02338d6c(u32 *p) {
    UnkStruct_02338d6c_Cam *c = &gCamera;
    u32 fl = c->unkC8;
    s32 v;

    if (fl & 0x200) {
        return FALSE;
    }
    if (!(fl & 2)) {
        if ((!(fl & 0x20000) && (*p & 2)) || ((fl & 0x20000) && (*p & 4))) {
            if (c->unk8 <= c->unk50) {
                c->unk8 = c->unk50;
                return TRUE;
            }
        } else {
            v = gPlayers[c->unk3A]->unk14 - c->unk20 * 0x60;
            if (c->unk8 >= v) {
                c->unk8 = v;
                return TRUE;
            }
        }
        return FALSE;
    }
    if ((!(fl & 0x20000) && (*p & 2)) || ((fl & 0x20000) && (*p & 4))) {
        v = gPlayers[c->unk3A]->unk14 - c->unk20 * 0x58;
        if (c->unk8 <= v) {
            c->unk8 = v;
            return TRUE;
        }
    } else {
        v = c->unk58 - c->unk20 * 0xc0;
        if (c->unk8 >= v) {
            c->unk8 = v;
            return TRUE;
        }
    }
    return FALSE;
}
