#include <nitro/types.h>
#include <main_sdk2/mbbeacon.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
u16 func_02017994(const void *p, u32 size);
s32 func_02012f9c(void *cb, const void *data, u32 size, u32 a, u32 b, u8 c);

s32 func_02017cf4(u32 a, u32 b, u8 flags)
{
    u8 *end = data_02080e74.obj + 0x358;
    if (data_02080e74.cur + 0x62 <= end) {
        data_02080ea0.len = 0x62;
    } else {
        data_02080ea0.len = end - data_02080e74.cur;
        {
            vu16 z = 0;
            MIi_CpuClear16(z, data_02080eae + data_02080ea0.len, 0x62 - data_02080ea0.len);
        }
    }
    MIi_CpuCopy16(data_02080e74.cur, data_02080eae, data_02080ea0.len);
    {
        u8 *obj;
        data_02080ea0.unkA = data_02080e74.unkF;
        data_02080ea0.unkB = data_02080e74.unk10;
        obj = data_02080e74.obj;
        data_02080ea0.lo = obj[0x4b2];
        data_02080ea0.unk5 = obj[0x4b3];
        data_02080ea0.unk6 = data_02080e74.unkE;
        data_02080ea0.ggid = *(u32 *)(obj + 0x4b8);
        data_02080ea0.hi = obj[0x4b5];
    }
    data_02080ea0.seq = data_02080e74.unk11++;
    data_02080ea0.sum = 0;
    data_02080ea0.sum = func_02017994(&data_02080ea0.sum, 0x68);
    data_02080e74.unkF++;
    if (data_02080e74.unkF < data_02080e74.unk10) {
        data_02080e74.cur += 0x62;
    } else {
        data_02080e74.state = 4;
    }
    return func_02012f9c(NULL, &data_02080ea0, sizeof(UnkMbBeacon), a, b, flags | 2);
}
