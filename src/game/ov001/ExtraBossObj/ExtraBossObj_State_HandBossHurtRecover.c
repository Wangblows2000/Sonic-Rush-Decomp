#include <nitro/types.h>

typedef struct UnkStruct_022ef8c0_o {
    u8 pad0[0x14];
    u32 flags;
} UnkStruct_022ef8c0_o;

typedef struct UnkStruct_022ef8c0 {
    u8 pad0[0x50];
    s32 vx;
    s32 vy;
    u8 pad58[0xa4 - 0x58];
    UnkStruct_022ef8c0_o oA4;
    u8 padBC[0xdc - 0xbc];
    UnkStruct_022ef8c0_o oDC;
    u8 padF4[0x1c8 - 0xf4];
    s16 unk1C8;
    u8 pad1CA[0x1d0 - 0x1ca];
    s32 timer;
} UnkStruct_022ef8c0;

extern s32 data_ov001_0231f070;
extern void *gExtraBossArchive;
extern s32 data_ov001_0231d4ac[];
extern u16 data_ov001_02316804[];
extern u16 data_ov001_023167f8[];
extern u16 data_ov001_023167ec[];

void ExtraBoss_HandBoss_SelectNextAttack(UnkStruct_022ef8c0 *p);
void ExtraBoss_LoadModelPalettes(s32 a, void *b, s32 *c, u16 *d, u16 *e, s32 n);

void ExtraBossObj_State_HandBossHurtRecover(UnkStruct_022ef8c0 *p) {
    p->timer--;
    if (p->timer <= 0) {
        p->oA4.flags &= ~0x200;
        p->oDC.flags &= ~0x200;
        ExtraBoss_HandBoss_SelectNextAttack(p);
        ExtraBoss_LoadModelPalettes(data_ov001_0231f070, gExtraBossArchive, data_ov001_0231d4ac, data_ov001_02316804, data_ov001_023167f8, 6);
        p->vx = p->vy = 0;
        return;
    }
    p->unk1C8++;
    if ((p->unk1C8 & 7) == 1)
        ExtraBoss_LoadModelPalettes(data_ov001_0231f070, gExtraBossArchive, data_ov001_0231d4ac, data_ov001_023167ec, data_ov001_023167f8, 6);
    else if ((p->unk1C8 & 7) == 5)
        ExtraBoss_LoadModelPalettes(data_ov001_0231f070, gExtraBossArchive, data_ov001_0231d4ac, data_ov001_02316804, data_ov001_023167f8, 6);
}
