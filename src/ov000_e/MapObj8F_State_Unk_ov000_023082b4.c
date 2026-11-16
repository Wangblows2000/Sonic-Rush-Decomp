#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
} UnkStruct_023082b4_T;

typedef struct {
    u8 pad[0xC8];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

void *func_ov000_02307e84(void);
UnkStruct_023082b4_T *func_ov000_0234237c(s32 a, void *b);
BOOL func_ov000_02307dfc(void *a, s32 b);
void Camera_TrySwitchScreen(void);
void MapObj8F_Unk_ov000_02308110(UnkStruct_StageObj *s);

void MapObj8F_State_Unk_ov000_023082b4(UnkStruct_StageObj *s) {
    UnkStruct_023082b4_T *t = func_ov000_0234237c(1, func_ov000_02307e84());

    if (t == NULL) {
        s->unk54 |= 4;
        return;
    }
    if (func_ov000_02307dfc(t, 0)) {
        s->unk1F4 &= ~0x10;
        s->unk54 |= 4;
        return;
    }
    if ((gCamera.unkC8 & 1) && !(s->unk1E0->unk4 & 2))
        Camera_TrySwitchScreen();
    if (t->unk20C & 0x2000)
        MapObj8F_Unk_ov000_02308110(s);
}
