#include <nitro/types.h>

typedef struct UnkStruct_02307890_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_02307890_o;

typedef struct UnkStruct_02307890 {
    u8 pad0[0x108];
    UnkStruct_02307890_o objs[0x19];
    u8 pad810[0xbd8 - 0x810];
    s32 timers[7];
    u8 padBF4[0xcd8 - 0xbf4];
    s32 counts[7];
} UnkStruct_02307890;

extern UnkStruct_02307890 *data_ov001_0231f5bc;

BOOL func_0202e5e4(float a, float b);
BOOL func_0202e528(float a, float b);
void func_ov001_02307778(s32 i, float scale, s16 *px, s16 *py);

void func_ov001_02307890(s32 i, s32 t) {
    UnkStruct_02307890 *g = data_ov001_0231f5bc;
    UnkStruct_02307890_o *o;
    float f;

    if (t < i * 0x19)
        return;
    o = &g->objs[i + 0xb];
    f = g->timers[i] / 60.0f;
    g->timers[i]++;
    if (func_0202e5e4(f, 1.0f))
        f = 1.0f;
    f = 1.0f - f;
    f = 120.0f - 70.0f * (1.0f - f * (f * f));
    if (func_0202e528(f, 50.0f))
        f = 50.0f;
    func_ov001_02307778(i, f, &o->x, &o->y);
}
