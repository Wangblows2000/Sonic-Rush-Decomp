#include <nitro/types.h>

typedef struct {
    u32 unk0;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x68 - 5];
    void *unk68;
} UnkStruct_023041f4;

extern UnkStruct_0236aad8 gCamera;

void MapObj74_State_Unk_ov000_023041bc(UnkStruct_023041f4 *p);

void MapObj74_Action_Unk_ov000_023041f4(UnkStruct_023041f4 *p) {
    if (p->unk4 != 0) {
        p->unk4 = 0;
        gCamera.unk0 &= ~0x4000;
    }
    p->unk68 = MapObj74_State_Unk_ov000_023041bc;
}
