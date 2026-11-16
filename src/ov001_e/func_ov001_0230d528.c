#include <nitro/types.h>

typedef struct UnkStruct_0230d528_m {
    s32 m[12];
} UnkStruct_0230d528_m;

extern s16 data_0206e470[];

void func_02001c00(const UnkStruct_0230d528_m *a, UnkStruct_0230d528_m *b, s32 x, s32 y, s32 z);
void func_02001500(UnkStruct_0230d528_m *m, s32 s, s32 c);
void func_0200160c(const UnkStruct_0230d528_m *a, const UnkStruct_0230d528_m *b, UnkStruct_0230d528_m *ab);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov001_0230d528(UnkStruct_0230d528_m *m, s32 d, u16 ang) {
    UnkStruct_0230d528_m t;

    func_02001c00(m, m, 0, d, 0);
    func_02001500(&t, SIN(ang), COS(ang));
    func_0200160c(&t, m, m);
    func_02001c00(m, m, 0, -d, 0);
}
