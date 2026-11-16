#include <nitro/types.h>

typedef struct { u8 unk0[0x7C]; u32 unk7C; } UnkStruct_0202ff80;

extern u32 data_02087cb8;
extern UnkStruct_0202ff80 data_0208f754;

void func_02004310(void);
void func_02004180(void);

static inline s32 GetStat_0202ff80(u32 *out) {
    if (*(vu32 *)0x04000600 & 0x4000) return -1;
    *out = *(vu32 *)0x04000600 & 0x8000;
    return 0;
}

static inline u32 GetPow_0202ff80(void) {
    return *(vu16 *)0x04000304 & 0x20E;
}

void func_0202ff80(void) {
    u32 v = data_02087cb8 & ~0x70000000;
    data_02087cb8 = v;
    if ((GetPow_0202ff80() & 0xC) == 0) return;
    if (*(vu16 *)0x04000060 & 0x1000) data_02087cb8 = v | 0x10000000;
    if (*(vu16 *)0x04000060 & 0x2000) data_02087cb8 |= 0x20000000;
    if (GetStat_0202ff80(&data_0208f754.unk7C) == 0 && data_0208f754.unk7C != 0) data_02087cb8 |= 0x40000000;
    func_02004310();
    func_02004180();
}
