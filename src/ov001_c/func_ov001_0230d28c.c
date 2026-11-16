#include <nitro/types.h>

typedef struct UnkStruct_0230d28c_m {
    s32 m[12];
} UnkStruct_0230d28c_m;

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void func_0200148c(UnkStruct_0230d28c_m *m);
void func_0200153c(UnkStruct_0230d28c_m *m, s32 s, s32 c);
void func_0200160c(const UnkStruct_0230d28c_m *a, const UnkStruct_0230d28c_m *b, UnkStruct_0230d28c_m *ab);
void func_02001c00(const UnkStruct_0230d28c_m *a, UnkStruct_0230d28c_m *b, s32 x, s32 y, s32 z);
u16 func_ov001_0230d650(s32 a, s32 b);
void func_ov001_0230d5bc(UnkStruct_0230d28c_m *m, s32 a, u16 b);
void func_ov001_0230d528(UnkStruct_0230d28c_m *m, s32 a, u16 b);

void func_ov001_0230d28c(UnkStruct_0230d28c_m *m, s32 mode, s32 flag, s32 force, u32 ang, s32 y, s32 z) {
    UnkStruct_0230d28c_m t;
    UnkStruct_0230d28c_m t2;
    s32 idx;

    if (force == 1 || (flag != 0 && (ang < 0x38e3 || ang > 0xc71c))) {
        func_0200148c(m);
        idx = (s32)(ang & 0xffff) >> 4;
        func_0200153c(&t, SIN_IDX(idx), COS_IDX(idx));
        func_0200160c(&t, m, m);
        func_02001c00(m, m, 0, y - 0x1e000, 0);
    } else {
        func_0200148c(m);
        func_02001c00(m, m, 0, y, 0);
        idx = (s32)(ang & 0xffff) >> 4;
        func_0200153c(&t, SIN_IDX(idx), COS_IDX(idx));
        func_0200160c(&t, m, m);
        func_02001c00(m, m, 0, -0x1e000, 0);
    }
    switch (mode) {
    case 0:
    case 5:
    case 6:
        func_02001c00(m, m, 0, 0, -z);
        break;
    case 1:
        func_0200148c(&t2);
        func_ov001_0230d5bc(&t2, -0xbf000, func_ov001_0230d650(0xbf000, z));
        func_0200160c(m, &t2, m);
        break;
    case 2:
        func_0200148c(&t2);
        func_ov001_0230d5bc(&t2, 0xbf000, -func_ov001_0230d650(0xbf000, z));
        func_0200160c(m, &t2, m);
        break;
    case 3:
        func_0200148c(&t2);
        func_ov001_0230d528(&t2, 0xbf000, func_ov001_0230d650(0xbf000, z));
        func_0200160c(m, &t2, m);
        break;
    case 4:
        func_0200148c(&t2);
        func_ov001_0230d528(&t2, -0xbf000, -func_ov001_0230d650(0xbf000, z));
        func_0200160c(m, &t2, m);
        break;
    }
}
