#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
} UnkWork_023353dc;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x68 - 0xc];
    s32 unk68;
    s32 unk6C;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

void func_ov000_023350bc(UnkWork_023353dc *w, s32 idx, u32 flags);

void func_ov000_023353dc(UnkWork_023353dc *w) {
    if (gCamera.unk4 != w->unkC || gCamera.unk8 != w->unk10)
        func_ov000_023350bc(w, 0, 5);
    if (gCamera.unk68 != w->unk14 || gCamera.unk6C != w->unk18)
        func_ov000_023350bc(w, 1, 5);
}
