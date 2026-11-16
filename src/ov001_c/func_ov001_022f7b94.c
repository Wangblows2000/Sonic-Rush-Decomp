#include <nitro/types.h>

typedef struct UnkStruct_022f7b94_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_022f7b94_o;

typedef struct UnkStruct_022f7b94 {
    u8 pad0[0x18];
    UnkStruct_022f7b94_o objs[0x17];
    u8 pad690[0xe38 - 0x690];
    u32 unkE38;
    u8 padE3C[0xe76 - 0xe3c];
    s16 unkE76[6];
    u16 digits[6];
    u8 padE8E[2];
    s32 unkE90;
    u8 padE94[0xea0 - 0xe94];
    void *unkEA0;
} UnkStruct_022f7b94;

typedef struct UnkStruct_022f7b94_t {
    u8 pad0[8];
    void (*fn)(void);
} UnkStruct_022f7b94_t;

extern UnkStruct_022f7b94 *data_ov001_0231f084;
extern UnkStruct_022f7b94_t *data_022b4574;
extern u32 data_0208fe70;

void func_ov001_022f7880(void);
void func_ov001_022f7790(void);
void func_ov001_022f7924(void);
void func_0201c070(void **pp, s32 a);
void func_02036754(u32 *p, u32 a, u32 b);
s32 func_02002094(s32 n, s32 d);
void func_02032d2c(void *p);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov001_022f7b94(void) {
    s32 i;
    s16 x;
    UnkStruct_022f7b94_o *o;

    if (data_ov001_0231f084->unkE38 & 3) {
        data_022b4574->fn = func_ov001_022f7880;
        func_ov001_022f7880();
        data_ov001_0231f084->unkE38 |= 0x4000;
        func_0201c070(&data_ov001_0231f084->unkEA0, 0);
        func_02036754(0, 8, 2);
        return;
    }
    data_ov001_0231f084->unkE90--;
    if (data_ov001_0231f084->unkE90 & 1) {
        for (i = 0; i < 6; i++)
            data_ov001_0231f084->digits[i] = Rand() % 10;
    }
    o = &data_ov001_0231f084->objs[0x16];
    o->x = 0x56;
    o->y = 0x7c;
    func_02032d2c(o);
    for (i = 0, x = 0x60; i < 6; i++, x += 10) {
        o = &data_ov001_0231f084->objs[data_ov001_0231f084->digits[i] + 10];
        o->x = x;
        o->y = 0x7c;
        func_02032d2c(o);
    }
    if (data_ov001_0231f084->unkE90 <= 0) {
        data_022b4574->fn = func_ov001_022f7924;
        data_ov001_0231f084->unkE90 = 0;
    }
    func_ov001_022f7790();
}
