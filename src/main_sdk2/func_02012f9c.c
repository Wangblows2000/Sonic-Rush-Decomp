#include <nitro/types.h>
#include <main_sdk2/wm.h>

extern u8 data_02080580[];

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);

s32 func_02012f9c(void *cb, const void *data, u32 size, u32 a, u16 b, u8 c)
{
    s32 r = func_02010a50(2, 7, 9);
    if (r != 0) {
        return r;
    }
    if (data == NULL) {
        return 6;
    }
    if (size > 0x70) {
        return 6;
    }
    MIi_CpuCopy16(data, data_02080580, size);
    DC_StoreRange(data_02080580, size);
    func_02010cd4(0x18, cb);
    r = func_02010b80(0x18, 5, data_02080580, size, a, b, c);
    if (r == 0) {
        r = 2;
    }
    return r;
}
