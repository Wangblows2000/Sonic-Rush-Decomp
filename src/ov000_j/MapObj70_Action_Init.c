#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3a];
    s8 unk3A;
    u8 pad3B[0x9e - 0x3b];
    s8 unk9E;
    u8 pad9F[0xc8 - 0x9f];
    u32 unkC8;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x214 - 0x6c];
    s8 unk214;
    s8 unk215;
} UnkStruct_02309e40;

extern UnkStruct_0236aad8 gCamera;

void MapObj70_State_Unk_ov000_02309ca4(UnkStruct_02309e40 *p);

void MapObj70_Action_Init(UnkStruct_02309e40 *p) {
    if (gCamera.unkC8 & 1) {
        p->unk214 = gCamera.unk3A;
        p->unk215 = gCamera.unk3A;
    } else {
        p->unk214 = gCamera.unk3A;
        p->unk215 = gCamera.unk9E;
    }
    p->unk68 = MapObj70_State_Unk_ov000_02309ca4;
}
