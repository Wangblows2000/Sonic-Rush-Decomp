#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    s8 unk20C;
    s8 unk20D;
} UnkWork_0231b9f0;

typedef struct {
    u8 pad0[0x3a];
    s8 unk3A;
    u8 pad3B[0x9e - 0x3b];
    s8 unk9E;
    u8 pad9F[0xc8 - 0x9f];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_0231b8f8(void);

void func_ov000_0231b9f0(UnkWork_0231b9f0 *w) {
    if (data_ov000_0236aad8.unkC8 & 1) {
        w->unk20C = data_ov000_0236aad8.unk3A;
        w->unk20D = data_ov000_0236aad8.unk3A;
    } else {
        w->unk20C = data_ov000_0236aad8.unk3A;
        w->unk20D = data_ov000_0236aad8.unk9E;
    }
    w->base.unk68 = func_ov000_0231b8f8;
}
