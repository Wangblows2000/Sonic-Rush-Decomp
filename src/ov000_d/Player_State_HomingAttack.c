#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_02348b28_3DC;

typedef struct UnkStruct_02348b28 {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0xC - 6];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u8 pad330[0x334 - 0x330];
    void (*unk334)(struct UnkStruct_02348b28 *w);
    u8 pad338[0x3DC - 0x338];
    UnkStruct_02348b28_3DC *unk3DC;
} UnkStruct_02348b28;

extern s16 data_0206e470[];

void Player_Action_Fall(UnkStruct_02348b28 *w);
void Player_Action_LandOnGround(UnkStruct_02348b28 *w, u8 a);
u16 FX_Atan2(s32 y, s32 x);

void Player_State_HomingAttack(UnkStruct_02348b28 *w) {
    s32 i;
    s32 v;

    if (w->unkC == 0) {
        w->unk58 |= 0x80;
        w->unk32C &= ~0x40;
        Player_Action_Fall(w);
        return;
    }
    w->unkC--;
    if (w->unk58 & 1) {
        w->unk58 |= 0x80;
        w->unk32C &= ~0x40;
        Player_Action_LandOnGround(w, 0);
        w->unk334(w);
        return;
    }
    if (w->unk3DC == NULL) {
        return;
    }
    i = (s32)((u16)(FX_Atan2((w->unk3DC->unk14 - w->unk14) >> 8, (w->unk3DC->unk10 - w->unk10) >> 8) + (w->unk5 << 8)) & 0xffff) >> 4;
    w->unk2E = data_0206e470[(u32)((i << 1) + 1)] >> 1;
    w->unk30 = data_0206e470[(u32)(i << 1)] >> 1;
    if (w->unk2E < 0) {
        w->unk5C |= 1;
    } else if (w->unk2E > 0) {
        w->unk5C &= ~1;
    }
    v = w->unk2E;
    if (v < 0) {
        v = -v;
    }
    if (v <= 0x100) {
        return;
    }
    if (!(w->unk58 & 4)) {
        return;
    }
    w->unk58 |= 0x80;
    w->unk32C &= ~0x40;
    Player_Action_LandOnGround(w, 0);
    w->unk334(w);
}
