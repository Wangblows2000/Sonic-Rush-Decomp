#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_Player;

typedef struct UnkStruct_022f5e74 UnkStruct_022f5e74;
struct UnkStruct_022f5e74 {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    u16 unk210;
    s16 unk212;
    u8 pad214[0x218 - 0x214];
    s32 unk218;
    u8 pad21C[0x35C - 0x21C];
    s32 unk35C;
    u8 pad360[0x4BE - 0x360];
    u16 unk4BE;
    s32 unk4C0;
    u16 unk4C4;
};

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Player *gPlayers;
extern s16 data_ov000_02363dc0[];
extern s16 data_ov000_02363dcc[];
extern u16 data_ov000_02363dbc[];
extern u16 data_ov000_02363db8[];

void func_ov000_022f685c(UnkStruct_022f5e74 *s);
void func_ov000_022f68c4(UnkStruct_022f5e74 *s, s32 a, s32 b);
void func_ov000_022f5e18(UnkStruct_022f5e74 *s);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.unk14;
}

static inline s32 GetIdxS(void) {
    switch (data_022c4560.unk10) {
    case 2:
    case 3:
        return 1;
    }
    return data_022c4560.unk14;
}

void func_ov000_022f5e74(UnkStruct_022f5e74 *s) {
    s32 px;
    s32 v;
    s32 i;

    if (s->unk4C4 != 0)
        s->unk4C4--;
    func_ov000_022f685c(s);
    s->unk210 += 0x800;
    s->unk212 = -s->base.unk2E << 3;
    px = gPlayers->unk10;
    if (s->unk218 < px && px < s->unk35C) {
        if (s->unk4C4 != 0) {
            if (s->unk4C0)
                s->base.unk2E -= 8;
            else
                s->base.unk2E += 8;
        } else {
            v = s->base.unk2E;
            if (v < 0) {
                if (-v > 8)
                    s->base.unk2E = v + 8;
                else
                    s->base.unk2E = 0;
            } else if (v > 0) {
                if (v > 8)
                    s->base.unk2E = v - 8;
                else
                    s->base.unk2E = 0;
            }
        }
        v = s->base.unk2E;
        if (v < -0x220)
            v = -0x220;
        else if (v > 0x220)
            v = 0x220;
        s->base.unk2E = v;
        i = GetIdxB();
        func_ov000_022f68c4(s, data_ov000_02363dc0[GetIdxS()], data_ov000_02363dcc[i]);
    } else {
        if (s->unk4C4 != 0) {
            if (px < s->base.unk10)
                s->base.unk2E -= 0x40;
            else
                s->base.unk2E += 0x40;
        } else {
            v = s->base.unk2E;
            if (v < 0) {
                if (-v > 0x40)
                    s->base.unk2E = v + 0x40;
                else
                    s->base.unk2E = 0;
            } else if (v > 0) {
                if (v > 0x40)
                    s->base.unk2E = v - 0x40;
                else
                    s->base.unk2E = 0;
            }
        }
        v = s->base.unk2E;
        if (v < -0x600)
            v = -0x600;
        else if (v > 0x600)
            v = 0x600;
        s->base.unk2E = v;
        func_ov000_022f68c4(s, 0x500, 0x500);
    }
    if (s->unk4C4 != 0)
        return;
    if (s->base.unk2E != 0)
        return;
    s->unk4BE++;
    if (s->unk4BE < data_ov000_02363dbc[GetIdxB()]) {
        s->unk4C0 = s->unk4C0 == 0 ? 1 : 0;
        s->unk4C4 = data_ov000_02363db8[GetIdxB()];
        s->base.unk68 = func_ov000_022f5e74;
    } else {
        s->base.unk68 = func_ov000_022f5e18;
    }
}
