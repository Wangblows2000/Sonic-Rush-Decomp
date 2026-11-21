#include <nitro/types.h>
#include <NitroSystem/b1dc.h>

s32 SND_ReadChannelInfo(void *p, u32 a, u32 b);

static inline UnkBuf_02081080 *func_0201b1dc_GetBuf(void)
{
    return data_02081040 < 0 ? NULL : &data_02081080[data_02081040];
}

s32 NNS_SndReadDriverChannelInfo(u32 a, u32 b)
{
    UnkBuf_02081080 *p = func_0201b1dc_GetBuf();
    if (p == NULL) {
        return 0;
    }
    return SND_ReadChannelInfo(p, a, b);
}
