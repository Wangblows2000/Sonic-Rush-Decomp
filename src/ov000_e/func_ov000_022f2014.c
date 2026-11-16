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
} UnkStruct_022f2014;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_Player *data_ov000_0236ac60;
extern UnkStruct_Mgr *data_ov000_02369b84;

void func_ov000_022f4ce4(UnkStruct_022f2014 *s, s32 a, s32 b, s32 c);
void func_ov000_022f8b70(u32 a, s32 b);
s32 func_020027a4(s32 y, s32 x);
s32 func_020021f0(s32 a);
BOOL func_ov000_022f8f78(void);
void func_ov000_0234915c(UnkStruct_Player *p, s16 a, s32 b);
void func_ov000_022f8df8(UnkStruct_022f2014 *s, s32 *x, s32 *y, s32 c);
void func_ov000_022f4850(void);
void func_ov000_023385e4(void);
void func_ov000_022f1f70(void);
void func_ov000_022f8b0c(void);

typedef struct {
    u8 pad[0x38];
    s32 unk38;
    u8 pad3C[0x74 - 0x3C];
    s32 unk74;
} UnkStruct_0236a48c;

extern UnkStruct_0236a48c data_ov000_0236a48c;

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov000_022f2014(UnkStruct_022f2014 *s) {
    s32 dx;
    s32 dy;

    func_ov000_022f4ce4(s, 0x1a, 0x1000, 0);
    func_ov000_022f8b70(data_022c4560.unk0, 0);
    dx = s->unk3C4 - s->unk424;
    dy = s->unk3C8 - s->unk428;
    s->unk438 = func_020027a4(dy, dx) + 0x8000;
    dx = s->unk3C4 - (data_ov000_0236ac60->base.unk10 << 4);
    dy = s->unk3C8 - (-data_ov000_0236ac60->base.unk14 << 4);
    s->unk43A = func_020027a4(dy, dx) + 0x8000;
    dx = s->unk3C4 - (data_ov000_0236ac60->base.unk10 << 4);
    dy = s->unk3C8 - (-data_ov000_0236ac60->base.unk14 << 4);
    s->unk43C = func_020021f0(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    data_ov000_0236a48c.unk74 = 0x2000;
    data_ov000_0236a48c.unk38 = 0x2000;
    if (!func_ov000_022f8f38()) {
        s->base.unk1B8 = NULL;
    } else {
        data_ov000_0236ac60->base.unk70 = func_ov000_022f8b0c;
        data_ov000_0236ac60->base.unk5C &= ~0x40;
    }
    data_ov000_02369ac0.unk4 = 0x34;
    s->base.unk68 = func_ov000_022f1f70;
}
