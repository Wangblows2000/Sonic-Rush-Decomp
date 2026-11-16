#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[9]; s8 unkD; } UnkStruct_02056d8c;
u32 PlayerDemoBg_GetImageCount(void);
void *PlayerDemoBg_LoadPaletteRes(void);
void PlayerDemoBg_LoadPalette(UnkStruct_02056d8c *s, void *p);
void PlayerDemoBg_Unk_02056c98(void);
void PlayerDemoBg_State_ShowImage(void);
void PlayerDemoBg_State_Init(UnkStruct_02056d8c *s) { s->unkD = PlayerDemoBg_GetImageCount(); if (s->unkD == 0) { s->fn = PlayerDemoBg_Unk_02056c98; return; } PlayerDemoBg_LoadPalette(s, PlayerDemoBg_LoadPaletteRes()); s->fn = PlayerDemoBg_State_ShowImage; }

#pragma thumb reset
