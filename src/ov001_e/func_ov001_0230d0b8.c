#include <nitro/types.h>

typedef struct UnkStruct_0230d0b8 {
    s32 unk0;
    u8 pad4[4];
    u8 mtx[0x30];
} UnkStruct_0230d0b8;

extern UnkStruct_0230d0b8 *data_ov001_0231f654;

u16 func_ov001_0230d650(s32 a, s32 b);
void func_ov001_0230d5bc(void *m, s32 d, u16 ang);

void func_ov001_0230d0b8(void) {
    UnkStruct_0230d0b8 *g = data_ov001_0231f654;
    u16 a = -func_ov001_0230d650(0xbf000, g->unk0);

    func_ov001_0230d5bc(g->mtx, -0xbf000, a);
}
