#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD5E - 0x960 - sizeof(UnkStruct_Mdl960)];
    u16 unkD5E;
    u8 padD60[0x1118 - 0xD60];
    s32 unk1118;
    s32 unk111C;
} UnkStruct_Boss;

void Boss6_SetCameraSideShot(UnkStruct_Boss *w);
void Boss6_SetCameraAfterFlip(UnkStruct_Boss *w);
void Boss6_UpdateFlashPalettes(s32 a);
void Boss6_SelectNextAttack(s32 a);

void Boss6_State_HurtFlip(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    w->unk111C--;
    if (w->unk111C == 0)
        Boss6_SetCameraSideShot(w);
    if (m->unk10C & 2) {
        Boss6_SetCameraAfterFlip(w);
        w->unkD5E ^= 1;
        w->unk20C &= ~4;
        Boss6_UpdateFlashPalettes(4);
        Boss6_SelectNextAttack(2);
        return;
    }
    w->unk1118++;
    Boss6_UpdateFlashPalettes(w->unk1118);
}
