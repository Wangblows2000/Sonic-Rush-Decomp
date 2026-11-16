#include <nitro/types.h>

typedef struct UnkStruct_02308d64 {
    u8 pad0[0xbbc];
    s32 unkBBC;
    u8 padBC0[0xbd0 - 0xbc0];
    s32 timer;
} UnkStruct_02308d64;

extern UnkStruct_02308d64 *data_ov001_0231f5bc;
extern s16 data_0208922c[];
extern s16 data_02087d04[];

void func_ov001_0230d6f4(s32 v);
void *func_ov001_0230c0a4(void);
void func_ov001_02312de0(void *p);
void func_ov001_0230c0e0(s32 v);

void func_ov001_02308d64(void) {
    UnkStruct_02308d64 *g = data_ov001_0231f5bc;
    s16 b;

    g->timer++;
    if (g->timer >= 0xb4) {
        b = g->timer - 0xb4;
        if (b > 0x10)
            b = 0x10;
        data_0208922c[0x24] = b;
        data_02087d04[0x24] = b;
    }
    if (g->timer <= 0x3c)
        func_ov001_0230d6f4(12288.0f * (1.0f - g->timer / 60.0f));
    switch (g->timer) {
    case 1:
        func_ov001_02312de0(func_ov001_0230c0a4());
        g->unkBBC = 0;
        return;
    case 200:
        g->timer = -1;
        func_ov001_0230c0e0(0);
        return;
    }
}
