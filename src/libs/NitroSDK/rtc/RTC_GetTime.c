#include <nitro/types.h>

s32 RTC_GetTimeAsync(u32 a, void (*cb)(u32), u32 c);
void RtcGetResultCallback(u32 v);
void RtcWaitBusy(void);
typedef struct { u8 pad[0x20]; s32 x20; } UnkStruct_0207f0e4;
extern UnkStruct_0207f0e4 data_0207f0e4;
s32 RTC_GetTime(u32 a) { s32 r = RTC_GetTimeAsync(a, RtcGetResultCallback, 0); data_0207f0e4.x20 = r; if (r == 0) RtcWaitBusy(); return data_0207f0e4.x20; }
