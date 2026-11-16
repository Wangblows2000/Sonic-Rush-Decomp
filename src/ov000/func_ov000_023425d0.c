#include <nitro/types.h>

typedef struct UnkStruct_023425d0 {
    u32 unk0;
    u8 pad[0x60];
    u32 unk64;
} UnkStruct_023425d0;

extern UnkStruct_023425d0 gCamera;

void func_ov000_023425d0(void) {
    gCamera.unk0 &= ~0x4000000;
    gCamera.unk64 &= ~0x4000000;
}
