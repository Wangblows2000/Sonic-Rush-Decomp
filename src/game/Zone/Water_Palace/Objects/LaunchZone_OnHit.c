#include <nitro/types.h>

typedef struct UnkStruct_0231d1c0_3 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_0231d1c0_3;

typedef struct UnkStruct_0231d1c0_2 {
    u8 pad[0x1e0];
    UnkStruct_0231d1c0_3 *unk1E0;
} UnkStruct_0231d1c0_2;

typedef struct UnkStruct_0231d1c0 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_0231d1c0;

void Player_Action_ZoneLaunch(void *t, s16 vx, s16 vy);

void LaunchZone_OnHit(UnkStruct_0231d1c0 *a, UnkStruct_0231d1c0 *b) {
    UnkStruct_0231d1c0_2 *s = a->unk18;
    void *t = b->unk18;
    s16 vx = 0;
    s16 vy = 0;
    u16 fl;
    if (t == NULL)
        return;
    fl = s->unk1E0->unk4;
    if (!(fl & 8)) {
        if (fl & 1)
            vx = -0xa00;
        else
            vx = 0xa00;
    }
    if (fl & 2)
        vy = -0xa00;
    if (fl & 4)
        vy = 0xa00;
    if (((fl & 2) | 4) && !(fl & 8)) {
        vx = (vx * 0xb5) >> 8;
        vy = (vy * 0xb5) >> 8;
    }
    Player_Action_ZoneLaunch(t, vx, vy);
}
