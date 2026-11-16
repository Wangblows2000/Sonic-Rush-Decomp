#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_0232dad8_P;

typedef struct {
    u8 pad0[6];
    s8 unk6;
    s8 unk7;
    u8 pad8[0x10 - 8];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x44 - 0x18];
    s16 unk44;
    s16 unk46;
    u8 pad48[0x62 - 0x48];
    u16 unk62;
    u8 pad64[0x94 - 0x64];
    u8 unk94[0x9C - 0x94];
    u16 unk9C;
    u8 pad9E[0x134 - 0x9E];
    u8 unk134[4];
} UnkStruct_0232dad8;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0232dad8 *work;
} UnkStruct_0232dad8_Task;

extern UnkStruct_0232dad8_Task *gTaskCurrent;
extern s32 gEnemyRoomHudTask;
extern UnkStruct_0232dad8_P *gPlayers[];

void StageObj_SetAnimation(void *w, s32 a);
void StageObj_Draw2D(void *w, void *a, void *b);
void StageObj_Draw2DEx(s32 x, s32 y, u8 rot, void *w, void *a, void *b, s32 c);

void FixVsHud_Draw(void) {
    UnkStruct_0232dad8 *w = gTaskCurrent->work;
    s8 a;
    s8 b;

    if (gEnemyRoomHudTask != 0)
        return;
    if (gPlayers[0]->unk10 > gPlayers[1]->unk10) {
        if (w->unk9C != 0) {
            w->unk62 = 0x80;
            StageObj_SetAnimation(w, 0);
        }
    } else if (w->unk9C == 0) {
        w->unk62 = 0x80;
        StageObj_SetAnimation(w, 1);
    }
    w->unk44 = (((w->unk62 & 0xf0) >> 4) << 3) + 0x100;
    w->unk46 = w->unk44;
    StageObj_Draw2D(w, w->unk94, NULL);
    a = w->unk6;
    b = w->unk7;
    w->unk6 = 0;
    w->unk7 = 0;
    w->unk46 = w->unk44 = 0x100;
    StageObj_Draw2DEx(w->unk10 + 0x1800, w->unk14, 0, w, w->unk134, NULL, 0);
    w->unk6 = a;
    w->unk7 = b;
}
