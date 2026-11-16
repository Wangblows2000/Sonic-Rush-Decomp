/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void ZoneBg_Zone2_TaskMain();
void ZoneBg_Zone2_TaskDestructor();
s32 Task_Create();

void ZoneBg_Zone2_Init(void) {
    Task_Create((u32)ZoneBg_Zone2_TaskMain, (u32)ZoneBg_Zone2_TaskDestructor, 0, 0x3f10, 3, 0);
}
