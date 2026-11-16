#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
} UnkStruct_022c4560;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x21C];
    void *unk21C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x328 - 0x20c];
    s16 unk328;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3C4 - 0x20C];
    s32 unk3C4;
    s32 unk3C8;
    u8 pad3CC[0x424 - 0x3CC];
    s32 unk424;
    s32 unk428;
    u8 pad42C[0x438 - 0x42C];
    u16 unk438;
    u16 unk43A;
    s32 unk43C;
} UnkStruct_022f1b74;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_Player *gPlayers;
extern UnkStruct_Mgr *data_ov000_02369b84;

void func_ov000_022f4ce4(UnkStruct_022f1b74 *s, s32 a, s32 b, s32 c);
void func_ov000_022f8b70(u32 a, s32 b);
s32 FX_Atan2(s32 y, s32 x);
s32 FX_Sqrt(s32 a);
BOOL func_ov000_022f8f78(void);
void func_ov000_0234915c(UnkStruct_Player *p, s16 a, s32 b);
void func_ov000_022f8df8(UnkStruct_022f1b74 *s, s32 *x, s32 *y, s32 c);
void func_ov000_022f4850(void);
void func_ov000_023385e4(void);
void func_ov000_022f1b4c(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022f1b74(UnkStruct_022f1b74 *s) {
    s32 dx;
    s32 dy;

    func_ov000_022f4ce4(s, 0x1e, 0x1000, 0);
    func_ov000_022f8b70(data_022c4560.unk0, 0);
    dx = s->unk3C4 - s->unk424;
    dy = s->unk3C8 - s->unk428;
    s->unk438 = FX_Atan2(dy, dx) + 0x8000;
    dx = s->unk3C4 - (gPlayers->base.unk10 << 4);
    dy = s->unk3C8 - (-gPlayers->base.unk14 << 4);
    s->unk43A = FX_Atan2(dy, dx) + 0x8000;
    dx = s->unk3C4 - (gPlayers->base.unk10 << 4);
    dy = s->unk3C8 - (-gPlayers->base.unk14 << 4);
    s->unk43C = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    if (func_ov000_022f8f78()) {
        if (gPlayers->unk328 != 0x11 && gPlayers->unk328 != 0x12) {
            func_ov000_0234915c(gPlayers, (0x1a000 - gPlayers->base.unk10) >> 7, -0x800);
            func_ov000_022f8df8(s, &gPlayers->base.unk10, &gPlayers->base.unk14, 0);
            gPlayers->base.unk70 = data_ov000_02369b84->unk21C;
            gPlayers->base.unk5C |= 0x40;
        }
        func_ov000_022f4850();
    }
    s->base.unk1B8 = NULL;
    func_ov000_023385e4();
    data_ov000_02369ac0.unk4 = 0x35;
    s->base.unk68 = func_ov000_022f1b4c;
}
