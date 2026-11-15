#include <nitro/types.h>

typedef struct UnkStruct_022f21f0 UnkStruct_022f21f0;
struct UnkStruct_022f21f0 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f21f0 *s);
    u8 pad1[0x434 - 0x6c];
    u16 unk434;
};

typedef struct UnkStruct_022f21f0_3 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022f21f0_3;

typedef struct UnkStruct_022f21f0_4 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022f21f0_4;

extern UnkStruct_022f21f0_3 data_022c4560;
extern UnkStruct_022f21f0_4 data_ov000_02369ac0;
extern u16 data_ov000_02363f9c[][8];

void func_ov000_022f4ce4(UnkStruct_022f21f0 *s, s32 a, s32 b, s32 c);
void func_ov000_02338624(s32 a);
void func_ov000_022f216c(UnkStruct_022f21f0 *s);

void func_ov000_022f21f0(UnkStruct_022f21f0 *s) {
    BOOL b;
    s32 i;
    func_ov000_022f4ce4(s, 0x1d, 0x1000, 1);
    b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = data_022c4560.unk14;
    s->unk434 = data_ov000_02363f9c[i][data_ov000_02369ac0.unk8 - 1];
    func_ov000_02338624(1);
    s->func = func_ov000_022f216c;
    s->func(s);
}
