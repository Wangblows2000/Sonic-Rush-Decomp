#include <nitro/types.h>

typedef struct {
    u8 pad0[0x324];
    u8 unk324;
    u8 unk325;
    u8 pad326[0x330 - 0x326];
    u32 unk330;
} UnkStruct_02333934;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_02333934_Sys;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_02333934_Stage;

extern UnkStruct_02333934_Sys data_022c4560;
extern UnkStruct_02333934_Stage data_ov000_0236aad8;

void *func_ov000_02333428(void *a, s32 b, s32 c, s32 d, s32 e);
void *EffBrakeSmoke3D_Create(void *a, s32 b, s32 c);
void *EffBrakeSmoke_Create(void *a, s32 b, s32 c);

static inline BOOL Stage_Busy(void) {
    if (data_ov000_0236aad8.unkE0 != 0 || data_ov000_0236aad8.unkE4 != 0)
        return TRUE;
    return FALSE;
}

void func_ov000_02333934(UnkStruct_02333934 *p) {
    if (p->unk325 != 0)
        return;
    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        if (Stage_Busy())
            func_ov000_02333428(p, 10, 0xe, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
        else
            EffBrakeSmoke3D_Create(p, 10, 0xe);
    } else if (p->unk330 & 0x1000) {
        EffBrakeSmoke_Create(p, 10, 0);
    } else {
        EffBrakeSmoke_Create(p, 10, 0xe);
    }
}
