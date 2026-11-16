/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossWork[];
void ExtraBoss_State_CharacterSwitch();
void ExtraBoss_PostUpdate_CharacterSwitch();
void ExtraBoss_Draw_CharacterSwitch();
extern u8 data_ov001_0231f520[];
extern u8 data_ov001_0231f524[];
s32 ExtraBoss_Unk_ov001_023063a0();
s32 ExtraBossObj_DestroyPlayer();
s32 ExtraBossObj_DestroyKind1();
s32 ExtraBoss_ClearSpawnLists();
s32 FX_ModS32();
s32 ExtraBoss_SetupSubDisplay();
s32 ExtraBoss_CopyCharacterDefaults();

void ExtraBoss_StartCharacterSwitch(void) {
    *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0x8) + 0x8) = (u32)ExtraBoss_State_CharacterSwitch;
    *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0xc) + 0x8) = (u32)ExtraBoss_PostUpdate_CharacterSwitch;
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0x10);
    *(u32 *)((u8 *)t0 + 0x8) = (u32)ExtraBoss_Draw_CharacterSwitch;
    *(u32 *)data_ov001_0231f520 = 0;
    *(u32 *)data_ov001_0231f524 = 0;
    ExtraBoss_Unk_ov001_023063a0(data_ov001_0231f524, data_ov001_0231f520, 0, t0);
    s32 t1 = (*(u32 *)*(u32 *)gExtraBossWork & ~4);
    s32 t2 = *(u32 *)gExtraBossWork;
    *(u32 *)t2 = t1;
    ExtraBossObj_DestroyPlayer(t1, t2);
    ExtraBossObj_DestroyKind1();
    ExtraBoss_ClearSpawnLists();
    s32 t3 = FX_ModS32(*(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0x40), 2);
    if (t3 == 0) {
        ExtraBoss_SetupSubDisplay(1);
    } else {
        ExtraBoss_SetupSubDisplay(0);
    }
    ExtraBoss_CopyCharacterDefaults(1);
    ExtraBoss_CopyCharacterDefaults(2);
}
