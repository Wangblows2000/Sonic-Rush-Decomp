#include <nitro/types.h>

typedef struct {
    u32 unk0;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x68 - 5];
    void *unk68;
} UnkStruct_02304184;

extern UnkStruct_0236aad8 gCamera;

void MapObj74_State_CameraLocked(UnkStruct_02304184 *p);

void MapObj74_Action_LockCamera(UnkStruct_02304184 *p) {
    if (p->unk4 == 0) {
        p->unk4 = 1;
        gCamera.unk0 |= 0x4000;
    }
    p->unk68 = MapObj74_State_CameraLocked;
}
