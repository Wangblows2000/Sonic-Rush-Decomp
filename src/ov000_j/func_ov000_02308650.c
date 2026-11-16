#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x1d - 4];
    u8 unk1D;
    u8 pad1E[0xc8 - 0x1e];
    u32 unkC8;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0x68];
    void *unk68;
} UnkStruct_02308650;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_02308404(UnkStruct_02308650 *p);

void func_ov000_02308650(UnkStruct_02308650 *p) {
    if (data_ov000_0236aad8.unkC8 & 1) {
        data_ov000_0236aad8.unkC8 |= 4;
        data_ov000_0236aad8.unk0 |= 0x200000;
        data_ov000_0236aad8.unk1D = 0x10;
        data_ov000_0236aad8.unkC8 |= 0x100;
    }
    p->unk68 = func_ov000_02308404;
}
