#include <nitro/types.h>

typedef struct UnkStruct_0230cbf8_m {
    s32 m[12];
} UnkStruct_0230cbf8_m;

extern s16 data_0206f470[];

void func_02001c00(const UnkStruct_0230cbf8_m *a, UnkStruct_0230cbf8_m *b, s32 x, s32 y, s32 z);
void func_02001500(UnkStruct_0230cbf8_m *m, s32 s, s32 c);
void func_0200160c(const UnkStruct_0230cbf8_m *a, const UnkStruct_0230cbf8_m *b, UnkStruct_0230cbf8_m *ab);

void func_ov001_0230cbf8(UnkStruct_0230cbf8_m *m) {
    UnkStruct_0230cbf8_m t;

    func_02001500(&t, data_0206f470[0], data_0206f470[1]);
    func_0200160c(&t, m, m);
    func_02001c00(m, m, 0, -0xbf000, 0);
}
