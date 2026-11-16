#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[8]; s8 unkC; u8 unkD[0x47]; u16 unk54; u16 unk56; } UnkStruct_02056d54;
void *PlayerDemoBg_LoadImageRes(s32 i);
void PlayerDemoBg_LoadImage(UnkStruct_02056d54 *s, void *p);
void PlayerDemoBg_HideImage(void);
void PlayerDemoBg_State_WaitShow(void);
void PlayerDemoBg_State_ShowImage(UnkStruct_02056d54 *s) { PlayerDemoBg_LoadImage(s, PlayerDemoBg_LoadImageRes(s->unkC)); PlayerDemoBg_HideImage(); s->unk54 = 0; s->unk56 = 6; s->fn = PlayerDemoBg_State_WaitShow; }

#pragma thumb reset
