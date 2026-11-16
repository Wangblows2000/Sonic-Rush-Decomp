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
    u8 pad44E[0x454 - 0x44E];
    u16 unk454;
    s16 unk456;
} UnkStruct_022ea454;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern u16 data_ov000_02363c44[][8];

void func_ov000_022ea120(void);
void func_ov000_022ea798(void);

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

u16 func_ov000_022ea454(UnkStruct_022ea454 *w) {
    u16 r = 1;

    w->unk44C++;
    if (w->unk454 != 0 && --w->unk454 == 0) {
        if (w->unk44C > data_ov000_02363c44[GetLevel()][data_ov000_02369ac0.unk8 - 1]) {
            w->unk44C = 0;
            w->unk456 = 0x3c;
            w->unk43C = func_ov000_022ea120;
            return 0;
        }
    }
    if (w->unk456 > 0) {
        w->unk456--;
        r |= 0x10;
    } else if (w->unk456 < 0) {
        w->unk456++;
        r |= 0x20;
    }
    if (w->base.unk58 & 1)
        w->unk43C = func_ov000_022ea798;
    return r;
}
