#include <nitro/types.h>

typedef struct { u8 pad0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
typedef struct { u32 unk0; u8 pad4[4]; u32 unk8; u8 padC[4]; u32 unk10; } UnkStruct_022c4590;
extern UnkStruct_022c4590 data_022c4590;
extern u8 data_02078b30[];
extern u8 data_02078b3c[];

typedef struct { s32 x; s32 y; } UnkVec_02046738;
typedef struct {
    u8 pad0[4];
    s32 unk4;
    UnkVec_02046738 pos[6]; /* 0x8 */
    s16 unk38[4];           /* 0x38 */
    u8 pad40[0x10];
    void *unk50;
    s16 digits[6][6];       /* 0x54 */
    s16 unk9C;
    u8 pad9E[2];
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
} UnkObj_02046738;

void MIi_CpuClear16(u16 v, void *dst, u32 size);
u32 func_02045b00(void);
u32 func_02045ab4(void);
s32 func_020480a0(u32 v);

#pragma thumb on
void func_02046738(UnkObj_02046738 *s) {
    s32 i;
    s32 j;
    u32 div;
    BOOL found;
    s16 *p;
    vu16 zero;
    u32 vals[6];

    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        for (i = 0; i < 4; i++) {
            s->pos[i].x = -0x72000;
            s->pos[i].y = 0x33ffe + i * 0x10000;
            s->unk38[i] = 0;
        }
    } else {
        for (i = 0; i < 4; i++) {
            s->pos[i].x = -0x72000;
            s->pos[i].y = 0x34000 + i * 0x10000;
            s->unk38[i] = 0;
        }
    }
    s->pos[4].x = -0x72000;
    s->pos[4].y = 0x6c000;
    s->pos[5].x = 0x56000;
    s->pos[5].y = 0x7c000;
    zero = 0;
    MIi_CpuClear16(zero, s->digits, sizeof(s->digits));
    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        vals[0] = -1;
        vals[1] = -1;
        vals[2] = func_02045b00();
        vals[3] = data_022c4590.unk10;
        vals[4] = 0;
        vals[5] = vals[2] + vals[3];
    } else {
        vals[0] = func_02045b00();
        vals[1] = func_02045ab4();
        vals[2] = data_022c4590.unk10;
        vals[3] = data_022c4590.unk8;
        vals[4] = 0;
        vals[5] = vals[0] + vals[1] + vals[2] + vals[3];
    }
    for (i = 0; i < 6; i++) {
        if (vals[i] > 999999)
            vals[i] = 999999;
    }
    s->unk9C = func_020480a0(vals[5]) + 0x23;
    data_022c4590.unk0 = vals[5];
    for (i = 0; i < 6; i++) {
        if (vals[i] != (u32)-1) {
            for (j = 0, found = FALSE, div = 100000, p = s->digits[i]; j < 6; j++, p++, div /= 10) {
                *p = vals[i] / div;
                vals[i] = vals[i] % div;
                if (p[0] == 0 && j != 5 && !found)
                    *p = -1;
                else
                    found = TRUE;
            }
        }
    }
    s->unkA4 = 0x50000;
    s->unkA8 = 0x8a000;
    s->unkA0 = 0x2000;
    s->unk4 = 0;
    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2)
        s->unk50 = data_02078b30;
    else
        s->unk50 = data_02078b3c;
}
#pragma thumb reset
