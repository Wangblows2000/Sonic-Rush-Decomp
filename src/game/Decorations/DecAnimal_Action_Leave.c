#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x2E - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0xBE - 0x32];
    s16 unkBE;
    u8 padC0[0x106 - 0xC0];
    s16 unk106;
} UnkStruct_0231d81c_E;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_0231d81c_T;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    u8 pad14[0x58 - 0x14];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x184 - 0x6C];
    UnkStruct_ObjParam *unk184;
    u8 pad188[0x190 - 0x188];
    UnkStruct_0231d81c_T *unk190;
} UnkStruct_0231d81c;

void DecAnimal_State_Leave(void);
void StageObj_SetAnimation(void *w, s32 a);
UnkStruct_0231d81c_E *EffBrakeSmoke_Create(void *w, s32 a, s32 b);

void DecAnimal_Action_Leave(UnkStruct_0231d81c *w) {
    UnkStruct_0231d81c_E *e;

    if (w->unk68 == DecAnimal_State_Leave)
        return;
    w->unk68 = DecAnimal_State_Leave;
    StageObj_SetAnimation(w, 2);
    if (w->unk184->unk2 != 5)
        w->unk5C |= 4;
    else
        w->unk58 |= 0x2000;
    if (w->unk190->unk10 < w->unk10)
        w->unk5C &= ~1;
    else
        w->unk5C |= 1;
    if (w->unk184->unk2 == 0xb) {
        e = EffBrakeSmoke_Create(w, 0, -6);
        if (e != NULL) {
            e->unk2E = 0;
            e->unk30 = 0x60;
            e->unkC = 0xfffe;
            e->unkBE >>= 2;
            e->unk106 >>= 2;
        }
    }
    w->unkC = 0;
}
