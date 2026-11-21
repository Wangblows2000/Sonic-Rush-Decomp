/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ZoneFx_Zone2_BuildPalettes();
s32 ZoneFx_Zone2_LoadSprites();
void ZoneFx_Zone2_CyclePalette();
s32 ZoneFx_Zone2_StartVAlarms();
void ZoneFx_Zone2_TaskMain();
void ZoneFx_Zone2_TaskDestructor();
s32 Task_Create();

void ZoneFx_Zone2_Init(u8 *p0) {
    ZoneFx_Zone2_BuildPalettes(p0);
    ZoneFx_Zone2_LoadSprites(p0);
    *(u32 *)((u8 *)p0 + 0x8b4) = (u32)ZoneFx_Zone2_CyclePalette;
    ZoneFx_Zone2_StartVAlarms((u32)ZoneFx_Zone2_CyclePalette);
    Task_Create((u32)ZoneFx_Zone2_TaskMain, (u32)ZoneFx_Zone2_TaskDestructor, 7, 0x3f10, 3, 0);
}
