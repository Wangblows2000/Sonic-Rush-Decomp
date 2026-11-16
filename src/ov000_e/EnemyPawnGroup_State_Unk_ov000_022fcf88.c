#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x18];
    u32 unk18;
    u8 pad1C[4];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

void EffBrakeSmoke_Create(UnkStruct_StageObj *w, s16 x, s16 y);
void func_ov000_022fcef4(UnkStruct_StageObj *w);

void EnemyPawnGroup_State_Unk_ov000_022fcf88(UnkStruct_StageObj *w) {
    if ((data_022c4560.unk20 & 7) == 0 && (data_022c4560.unk18 & 1))
        EffBrakeSmoke_Create(w, -0xd - (Rand() & 0xe), -6);
    if (w->unkC != 0) {
        w->unkC--;
        return;
    }
    func_ov000_022fcef4(w);
}
