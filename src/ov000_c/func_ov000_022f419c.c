#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x212];
    u16 unk212;
    u16 unk214;
} UnkStruct_022f419c;

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

extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_022f419c *data_ov000_02369b84;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02363eac[][2][4];
extern u8 data_ov000_02363ebc[][2][4];
extern u8 data_ov000_02363ecc[][2][4];
extern u8 data_ov000_02363eec[][2][4];
extern s8 data_ov000_02363da4[];
extern s8 data_ov000_02363e38[][4];
extern s8 data_ov000_02363dc4[][2];
extern s8 data_ov000_02363e10[][4];
extern s8 data_ov000_02363e28[][4];
extern s8 data_ov000_02363db4[];
extern s8 data_ov000_02363e30[][4];
extern s8 data_ov000_02363e18[][4];
extern s8 data_ov000_02363de0[];
extern s8 data_ov000_02363da8[];

void func_ov000_022f8964(s32 a);
void func_ov000_022f8920(s32 a);
void func_ov000_022f8860(s32 a);
void func_ov000_022f8838(s32 a);
void func_ov000_022f8810(s32 a);

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

void func_ov000_022f419c(void) {
    UnkStruct_022f419c *w = data_ov000_02369b84;
    u16 hp = data_ov000_02369ac0.unk8;

    if (hp >= 7) {
        switch (data_ov000_02363eac[GetLevel()][w->unk214][w->unk212]) {
        case 0:
            func_ov000_022f8964(data_ov000_02363da4[Rand() & 1]);
            break;
        case 1:
            func_ov000_022f8920(1);
            break;
        }
    } else if (hp >= 6) {
        switch (data_ov000_02363ebc[GetLevel()][w->unk214][w->unk212]) {
        case 0:
            func_ov000_022f8964(data_ov000_02363e38[GetLevel()][Rand() & 3]);
            break;
        case 1:
            func_ov000_022f8920(data_ov000_02363dc4[GetLevel()][Rand() & 1]);
            break;
        }
    } else if (hp >= 4) {
        switch (data_ov000_02363ecc[GetLevel()][w->unk214][w->unk212]) {
        case 0:
            func_ov000_022f8964(data_ov000_02363e10[GetLevel()][Rand() & 3]);
            break;
        case 1:
            func_ov000_022f8920(data_ov000_02363e28[GetLevel()][Rand() & 3]);
            break;
        case 3:
            func_ov000_022f8860(data_ov000_02363db4[Rand() & 1]);
            break;
        }
    } else if (hp >= 2) {
        switch (data_ov000_02363eec[GetLevel()][w->unk214][w->unk212]) {
        case 0:
            func_ov000_022f8964(data_ov000_02363e30[GetLevel()][Rand() & 3]);
            break;
        case 1:
            func_ov000_022f8920(data_ov000_02363e18[GetLevel()][Rand() & 3]);
            break;
        case 3:
            func_ov000_022f8860(data_ov000_02363de0[Rand() & 3]);
            break;
        case 4:
            func_ov000_022f8838(data_ov000_02363da8[Rand() & 1]);
            break;
        }
    } else {
        func_ov000_022f8810(0);
    }
    w->unk212++;
    if (w->unk212 >= 4) {
        w->unk212 = 0;
        w->unk214 ^= 1;
    }
}
