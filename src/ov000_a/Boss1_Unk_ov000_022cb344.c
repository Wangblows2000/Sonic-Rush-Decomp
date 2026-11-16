#include <nitro/types.h>

typedef struct UnkStruct_022cb344 {
    u8 pad[0x218];
    u16 unk218;
    u16 unk21A;
} UnkStruct_022cb344;

typedef struct UnkStruct_022cb344_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022cb344_4;

typedef struct UnkStruct_022cb344_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022cb344_7;

extern UnkStruct_022cb344 *gBoss1;
extern UnkStruct_022cb344_7 gBossStatus;
extern UnkStruct_022cb344_4 gGameState;
extern u32 gRandSeed;
extern u8 data_ov000_02362da4[][8];
extern u8 data_ov000_02362c84[][8];
extern u8 data_ov000_02362c64[][8];
extern u8 data_ov000_02362ca4[][8];
extern s16 data_ov000_02362d64[][8];
extern s16 data_ov000_02362d84[][8];
extern s16 data_ov000_02362cc4[][8];
extern u8 data_ov000_02362b64[];
extern u8 data_ov000_02362b68[];
extern s8 data_ov000_02362b9c[][4];

BOOL Boss1_Unk_ov000_022cc6d4(void);
void Boss1_Action_Unk_ov000_022cbf78(s32 a);
void Boss1_Action_Unk_ov000_022cbe88(s32 a);
void Boss1_Action_Unk_ov000_022cbf28(s32 a);
void Boss1_Action_Unk_ov000_022cbe38(s32 a);
void Boss1_Action_Unk_ov000_022cbed8(s32 a);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

static inline s32 GetIdx(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void Boss1_Unk_ov000_022cb344(void) {
    UnkStruct_022cb344 *g = gBoss1;
    if (gBossStatus.unk8 >= 6) {
        switch (data_ov000_02362da4[g->unk21A][g->unk218]) {
        case 0:
            if (Boss1_Unk_ov000_022cc6d4()) {
                if ((Rand() & 0xf) > 4)
                    Boss1_Action_Unk_ov000_022cbf78(data_ov000_02362d64[GetIdx()][Rand() & 7]);
                else
                    Boss1_Action_Unk_ov000_022cbe88(data_ov000_02362d64[GetIdx()][Rand() & 7]);
            } else {
                Boss1_Action_Unk_ov000_022cbf78(data_ov000_02362d64[GetIdx()][Rand() & 7]);
            }
            break;
        case 1:
            Boss1_Action_Unk_ov000_022cbf28(data_ov000_02362d84[GetIdx()][Rand() & 7]);
            break;
        }
    } else if (gBossStatus.unk8 >= 3) {
        u8 k;
        if (Boss1_Unk_ov000_022cc6d4())
            k = data_ov000_02362c84[g->unk21A][g->unk218];
        else
            k = data_ov000_02362c64[g->unk21A][g->unk218];
        switch (k) {
        case 0:
            if (Boss1_Unk_ov000_022cc6d4()) {
                if ((Rand() & 0xf) > 8)
                    Boss1_Action_Unk_ov000_022cbf78(data_ov000_02362d64[GetIdx()][Rand() & 7]);
                else
                    Boss1_Action_Unk_ov000_022cbe88(data_ov000_02362d64[GetIdx()][Rand() & 7]);
            } else {
                Boss1_Action_Unk_ov000_022cbf78(data_ov000_02362d64[GetIdx()][Rand() & 7]);
            }
            break;
        case 1:
            Boss1_Action_Unk_ov000_022cbf28(data_ov000_02362d84[GetIdx()][Rand() & 7]);
            break;
        case 2:
            if (Boss1_Unk_ov000_022cc6d4())
                Boss1_Action_Unk_ov000_022cbe38(data_ov000_02362b64[Rand() & 3]);
            else
                Boss1_Action_Unk_ov000_022cbed8(data_ov000_02362b9c[GetIdx()][Rand() & 3]);
            break;
        }
    } else {
        switch (data_ov000_02362ca4[g->unk21A][g->unk218]) {
        case 0:
            if (Boss1_Unk_ov000_022cc6d4()) {
                if ((Rand() & 0xf) > 10)
                    Boss1_Action_Unk_ov000_022cbf78(data_ov000_02362d64[GetIdx()][Rand() & 7]);
                else
                    Boss1_Action_Unk_ov000_022cbe88(data_ov000_02362d64[GetIdx()][Rand() & 7]);
            } else {
                Boss1_Action_Unk_ov000_022cbf78(data_ov000_02362d64[GetIdx()][Rand() & 7]);
            }
            break;
        case 1:
            Boss1_Action_Unk_ov000_022cbf28(data_ov000_02362d84[GetIdx()][Rand() & 7]);
            break;
        case 2:
            if (Boss1_Unk_ov000_022cc6d4())
                Boss1_Action_Unk_ov000_022cbe38(data_ov000_02362b68[Rand() & 3]);
            else
                Boss1_Action_Unk_ov000_022cbed8(data_ov000_02362cc4[GetIdx()][Rand() & 7]);
            break;
        }
    }
    g->unk218++;
    if (g->unk218 >= 8) {
        g->unk218 = 0;
        g->unk21A = Rand() & 3;
    }
}
