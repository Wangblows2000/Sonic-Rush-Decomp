#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    u8 pad28[0x5C - 0x28];
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x124 - 0x6C];
    void *unk124;
    u8 pad128[0x134 - 0x128];
    u8 unk134[0x324 - 0x134];
    u8 unk324;
    u8 unk325;
    u8 pad326[0x32C - 0x326];
    u32 unk32C;
    u8 pad330[0x394 - 0x330];
    s32 unk394;
    s32 unk398;
    s32 unk39C;
    s32 unk3A0;
    s32 unk3A4;
    s32 unk3A8;
    u8 pad3AC[0x3C4 - 0x3AC];
    s16 unk3C4;
} UnkStruct_0234360c;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0234360c *unk10;
} UnkStruct_022b4574;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x24 - 0x1E];
    u32 unk24;
} UnkStruct_022c4560;

typedef struct {
    u16 x;
    u16 y;
    u16 z;
} UnkStruct_02074960;

extern UnkStruct_022b4574 *gTaskCurrent;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02074960 data_02074960[][3];

void Player_State_HomingAttack(void);
void func_02022270(s32 idx, s32 color);
void StageObj_Draw3DEx(s32 x, s32 y, s32 z, s32 a, UnkStruct_0234360c *s, void *b);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Player_Draw(void) {
    UnkStruct_0234360c *s = gTaskCurrent->unk10;
    u16 saved = s->unk5C;

    if (s->unk124 == NULL)
        return;
    if (s->unk325 != 0)
        return;

    if (s->unk3C4 != 0 || ((s->unk68 == Player_State_HomingAttack || (s->unk32C & 0x100)) && !(s->unk32C & 0x80))) {
        if (s->unk324 != 1) {
            func_02022270(0, 0x6182);
            func_02022270(1, 0x6182);
            func_02022270(2, 0x6182);
        } else {
            func_02022270(0, 0x1a98);
            func_02022270(1, 0x1a98);
            func_02022270(2, 0x1a98);
        }
        s->unk5C |= 0x1010;
        if (s->unk32C & 0x80000000) {
            s->unk5C &= ~0x1000;
            s->unk32C &= ~0x80000000;
        }
        if (gGameState.unk24 & 1) {
            if (gGameState.unk24 & 2) {
                if (ABS(s->unk10 - s->unk394) >= 0x200 || ABS(s->unk14 - s->unk398) >= 0x200)
                    StageObj_Draw3DEx(s->unk394, s->unk398, s->unk39C, s->unk1, s, s->unk134);
            } else {
                if (ABS(s->unk10 - s->unk3A0) >= 0x200 || ABS(s->unk14 - s->unk3A4) >= 0x200)
                    StageObj_Draw3DEx(s->unk3A0, s->unk3A4, s->unk3A8, s->unk1, s, s->unk134);
            }
        } else {
            if (ABS(s->unk10 - s->unk1C) >= 0x200 || ABS(s->unk14 - s->unk20) >= 0x200)
                StageObj_Draw3DEx(s->unk1C, s->unk20, s->unk24, s->unk1, s, s->unk134);
        }
        func_02022270(0, data_02074960[gGameState.unk1C][gGameState.unk1D].x);
        func_02022270(1, data_02074960[gGameState.unk1C][gGameState.unk1D].y);
        func_02022270(2, data_02074960[gGameState.unk1C][gGameState.unk1D].z);
        s->unk5C = saved;
    }

    s->unk3A0 = s->unk394;
    s->unk3A4 = s->unk398;
    s->unk3A8 = s->unk39C;
    s->unk394 = s->unk1C;
    s->unk398 = s->unk20;
    s->unk39C = s->unk24;
    if (s->unk32C & 0x4000000) {
        func_02022270(0, 0x7ef5);
        func_02022270(1, 0x4548);
        func_02022270(2, 0x2040);
    }
}
