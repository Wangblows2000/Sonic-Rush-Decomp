#include <nitro/types.h>

typedef struct { u32 unk0; } UnkStruct_0201cf4c;
extern UnkStruct_0201cf4c data_02083e10;

void func_0201cf04(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_FlushRange(void *start, u32 size);
BOOL func_0201cacc(s32 type, void *bufL, void *bufR, u32 bufSize, s32 format, s32 capA, s32 capB, BOOL loop,
                   s32 rate, s32 volume, s32 panL, s32 panR, u32 split, void *cb, void *arg);

BOOL func_0201cf4c(u8 *buf, u32 size, s32 format, s32 rate, u32 split, void *cb, void *arg) {
    vu32 z;
    func_0201cf04();
    if (data_02083e10.unk0 != 0) return FALSE;
    z = 0;
    MIi_CpuClear32(z, buf, size);
    DC_FlushRange(buf, size);
    return func_0201cacc(2, buf, buf + size / 2, size / 2, format, 0, 0, TRUE, rate, 0x7F, 0, 0x7F, split, cb, arg);
}
