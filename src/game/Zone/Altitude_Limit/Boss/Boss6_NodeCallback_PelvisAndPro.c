#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0xD5E - 0x210];
    u16 unkD5E;
    u8 padD60[0x10EC - 0xD60];
    u16 unk10EC;
} UnkStruct_Boss;

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
    u8 pad28[0x92 - 0x28];
    u8 unk92;
} UnkStruct_022e5f58;

extern UnkStruct_Boss *gBoss6;
extern void *data_ov000_023658bc[];
extern u8 data_ov000_023638c4[];

BOOL Boss6_StoreNodeMatrix(UnkStruct_022e5f58 *w, s32 a, void *b);
void G3_MultMtx33(void *m);

void Boss6_NodeCallback_PelvisAndPro(UnkStruct_022e5f58 *w) {
    u16 st = gBoss6->unk10EC;

    switch (st) {
    case 1:
        if (!Boss6_StoreNodeMatrix(w, 0x1c, data_ov000_023658bc[2]))
            return;
        gBoss6->unk10EC++;
        return;
    case 2:
        if (!Boss6_StoreNodeMatrix(w, 0x1d, data_ov000_023658bc[1]))
            return;
        gBoss6->unk10EC++;
        return;
    case 3:
        if (!Boss6_StoreNodeMatrix(w, 0x1e, data_ov000_023658bc[0]))
            return;
        gBoss6->unk10EC = 0;
        w->unk24 = 0;
        w->unk92 = 0;
        return;
    case 0:
        gBoss6->unk10EC = st + 1;
        if ((gBoss6->unkD5E != 0 && !(gBoss6->unk20C & 0x100)) || (gBoss6->unkD5E == 0 && (gBoss6->unk20C & 0x100)))
            G3_MultMtx33(data_ov000_023638c4);
        break;
    }
}
