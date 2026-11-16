/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 ZoneMap_InitMapBg();
s32 ZoneMap_Unk_ov001_022e5618();
s32 ZoneMap_Unk_ov001_022e4eb8();
s32 ZoneMap_IsBlaze();
s32 ZoneMap_LoadPlayerArchive();
s32 ZoneMap_InitScrollArrows();

void ZoneMap_InitWork(s32 p0) {
    ZoneMap_InitMapBg(((u8 *)p0 + 0x4), *(u32 *)((u8 *)p0 + 0x1cd4));
    ZoneMap_Unk_ov001_022e5618(((u8 *)p0 + 0x4), 1);
    ZoneMap_Unk_ov001_022e4eb8(((u8 *)p0 + 0x674), ((u8 *)p0 + 0x4), *(u32 *)((u8 *)p0 + 0x1ce0), *(u32 *)((u8 *)p0 + 0x1cd8));
    s32 t0 = ZoneMap_IsBlaze();
    ZoneMap_LoadPlayerArchive(((u8 *)p0 + 0x1994), t0, *(u32 *)((u8 *)p0 + 0x1ce8));
    ZoneMap_InitScrollArrows(((u8 *)p0 + 0x1b08), *(u32 *)((u8 *)p0 + 0x1cdc), *(u32 *)((u8 *)p0 + 0x1ce4), *(u32 *)((u8 *)p0 + 0x1cdc));
}

#pragma thumb reset
