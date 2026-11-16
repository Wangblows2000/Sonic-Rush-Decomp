#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x1d - 4];
    u8 unk1D;
    u8 pad1E[0xc8 - 0x1e];
    u32 unkC8;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0x68];
    void *unk68;
} UnkStruct_02308650;

extern UnkStruct_0236aad8 gCamera;

void MapObj8F_State_Unk_ov000_02308404(UnkStruct_02308650 *p);

void MapObj8F_Action_Unk_ov000_02308650(UnkStruct_02308650 *p) {
    if (gCamera.unkC8 & 1) {
        gCamera.unkC8 |= 4;
        gCamera.unk0 |= 0x200000;
        gCamera.unk1D = 0x10;
        gCamera.unkC8 |= 0x100;
    }
    p->unk68 = MapObj8F_State_Unk_ov000_02308404;
}
