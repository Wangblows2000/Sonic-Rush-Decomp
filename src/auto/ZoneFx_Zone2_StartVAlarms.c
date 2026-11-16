/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gZoneFxWork[];
void ZoneFx_Zone2_VAlarmTaskMain();
s32 Task_Create();
s32 ZoneFx_Zone2_InitVAlarms();

void ZoneFx_Zone2_StartVAlarms(void) {
    s32 t0 = *(u32 *)gZoneFxWork;
    Task_Create((u32)ZoneFx_Zone2_VAlarmTaskMain, 0, 7, 0xf014, 3, 0);
    ZoneFx_Zone2_InitVAlarms(t0);
}
