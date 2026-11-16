#include <nitro/types.h>

typedef struct UnkStruct_0230cc54_m {
    s32 m[12];
} UnkStruct_0230cc54_m;

extern s16 data_02071470[];

void func_02001c00(const UnkStruct_0230cc54_m *a, UnkStruct_0230cc54_m *b, s32 x, s32 y, s32 z);
void func_02001520(UnkStruct_0230cc54_m *m, s32 s, s32 c);
void func_0200160c(const UnkStruct_0230cc54_m *a, const UnkStruct_0230cc54_m *b, UnkStruct_0230cc54_m *ab);

void func_ov001_0230cc54(UnkStruct_0230cc54_m *m) {
    UnkStruct_0230cc54_m t;

    func_02001520(&t, data_02071470[0], data_02071470[1]);
    func_0200160c(&t, m, m);
    func_02001c00(m, m, -0xbf000, 0, 0);
}
