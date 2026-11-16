#include <nitro/types.h>

typedef struct { u32 a; s32 b; } UnkEnt_02076adc;
typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_020572b4;
extern UnkEnt_02076adc data_02076adc[];
extern UnkBlend_020572b4 data_0208926c;
extern UnkBlend_020572b4 data_02087d44;

void BrightnessFade_Create(u16 a, s16 b);

#pragma thumb on
void func_020572b4(u32 n, u32 flags) {
    u8 idx = flags & 3;

    if (n != 0) {
        if (flags & 4)
            BrightnessFade_Create(data_02076adc[idx].a, 0x1000 / n);
        else
            BrightnessFade_Create(data_02076adc[idx].a | 0x100, 0x1000 / n);
    } else {
        data_0208926c.unk8 = data_02076adc[idx].b;
        if (flags & 4)
            data_02087d44.unk8 = data_0208926c.unk8;
    }
}
#pragma thumb reset
