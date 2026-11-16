#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x43C - 0x20C];
    void *unk43C;
    u8 pad440[0x44C - 0x440];
    u16 unk44C;
    u16 unk44E;
    u8 pad450[0x454 - 0x450];
    u16 unk454;
    s16 unk456;
} UnkStruct_022ea798;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern u16 data_ov000_02363c44[][8];

BOOL func_ov000_022eb268(UnkStruct_022ea798 *w);
void func_ov000_022eb124(UnkStruct_022ea798 *w);
BOOL func_ov000_022eb158(UnkStruct_022ea798 *w);
void func_ov000_022eae84(UnkStruct_022ea798 *w);
void func_ov000_022ea900(UnkStruct_022ea798 *w);
BOOL func_ov000_022eabfc(UnkStruct_022ea798 *w);

typedef struct {
    u8 pad[0x220];
    u32 unk220;
} UnkStruct_Boss;

extern UnkStruct_Boss *data_ov000_02369b78;

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

u16 func_ov000_022ea798(UnkStruct_022ea798 *w) {
    UnkStruct_Boss *b = data_ov000_02369b78;

    if ((u32)(data_ov000_02369ac0.unk4 - 0x24) <= 1)
        w->unk44C++;
    if ((u32)(data_ov000_02369ac0.unk4 - 0x24) <= 1)
        w->unk44E++;
    if (func_ov000_022eb268(w)) {
        func_ov000_022eb124(w);
        return 0;
    }
    if (!(b->unk220 & 0x1f) && func_ov000_022eb158(w)) {
        func_ov000_022eae84(w);
        return 0;
    }
    if (func_ov000_022eabfc(w))
        return 0;
    if (w->unk44C <= data_ov000_02363c44[GetLevel()][data_ov000_02369ac0.unk8 - 1])
        return 0;
    w->unk44C = 0;
    func_ov000_022ea900(w);
    return 0;
}
