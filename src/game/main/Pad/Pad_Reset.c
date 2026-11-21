#include <nitro/types.h>

typedef struct {
    u8 unk0[0x22];
    u16 a[12];
    u16 b[12];
} UnkObj_020351d8;

void MIi_CpuClear16(u16 data, void *dst, u32 size);

void Pad_Reset(UnkObj_020351d8 *o, const u16 *a, const u16 *b) {
    s32 i;
    vu16 zero = 0;
    MIi_CpuClear16(zero, o, 0x52);
    if (a != NULL) {
        for (i = 0; i < 12; i++) {
            o->a[i] = a[i];
        }
    } else {
        for (i = 0; i < 12; i++) {
            o->a[i] = 0x14;
        }
    }
    if (b != NULL) {
        for (i = 0; i < 12; i++) {
            o->b[i] = b[i];
        }
    } else {
        for (i = 0; i < 12; i++) {
            o->b[i] = 2;
        }
    }
}
