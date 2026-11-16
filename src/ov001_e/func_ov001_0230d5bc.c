#include <nitro/types.h>

typedef struct UnkStruct_0230d5bc_m {
    s32 m[12];
} UnkStruct_0230d5bc_m;

extern s16 data_0206e470[];

void func_02001c00(const UnkStruct_0230d5bc_m *a, UnkStruct_0230d5bc_m *b, s32 x, s32 y, s32 z);
void func_02001520(UnkStruct_0230d5bc_m *m, s32 s, s32 c);
void func_0200160c(const UnkStruct_0230d5bc_m *a, const UnkStruct_0230d5bc_m *b, UnkStruct_0230d5bc_m *ab);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov001_0230d5bc(UnkStruct_0230d5bc_m *m, s32 d, u16 ang) {
    UnkStruct_0230d5bc_m t;

    func_02001c00(m, m, d, 0, 0);
    func_02001520(&t, SIN(ang), COS(ang));
    func_0200160c(&t, m, m);
    func_02001c00(m, m, -d, 0, 0);
}
