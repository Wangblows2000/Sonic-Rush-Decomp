#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    s8 unk210;
    u8 pad211[0x230 - 0x211];
    s16 unk230;
} UnkStruct_02307dfc;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x32C - 0x20C];
    u32 unk32C;
} UnkStruct_Player;

typedef struct {
    u8 pad[0xC8];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;
extern UnkStruct_Player *gPlayers[];

BOOL func_ov000_02307dfc(UnkStruct_02307dfc *t, s32 inc) {
    UnkStruct_Player *p;
    u32 v;

    if ((data_ov000_0236aad8.unkC8 & 1) || t == NULL)
        return FALSE;
    p = gPlayers[t->unk210];
    v = p->unk32C & 0x400;
    if ((t->unk20C & 0x10000000) && v == 0)
        return TRUE;
    if (v != 0) {
        if (inc != 0)
            t->unk230++;
        t->unk20C |= 0x10000000;
    }
    return FALSE;
}
