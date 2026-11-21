#include <nitro/types.h>
#include <main_sdk2/b1dc.h>

BOOL SND_IsFinishedCommandTag(u32 a);
void SND_ReadDriverInfo(void *p);
u32 SND_GetCurrentCommandTag(void);
void DC_InvalidateRange(void *p, u32 size);

BOOL NNS_SndUpdateDriverInfo(void)
{
    if (data_02081044 == 0) {
        if (!SND_IsFinishedCommandTag(data_02081048)) {
            return FALSE;
        }
        if (data_02081040 < 0) {
            data_02081040 = 1;
        }
        SND_ReadDriverInfo(&data_02081080[data_02081040]);
        data_02081048 = SND_GetCurrentCommandTag();
        if (data_02081040 == 0) {
            data_02081040 = 1;
        } else {
            data_02081040 = 0;
        }
        DC_InvalidateRange(&data_02081080[data_02081040], sizeof(UnkBuf_02081080));
        return TRUE;
    }
    SND_ReadDriverInfo(data_02081080);
    data_02081048 = SND_GetCurrentCommandTag();
    data_02081044 = 0;
    return FALSE;
}
