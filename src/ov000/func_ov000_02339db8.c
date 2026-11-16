#include <nitro/types.h>

typedef struct UnkStruct_02339db8 {
    u8 pad[0xc8];
    s32 unkC8;
    s32 unkCC;
} UnkStruct_02339db8;

extern UnkStruct_02339db8 gCamera;

void func_ov000_0233e668(void);

void func_ov000_02339db8(void) {
    gCamera.unkCC = gCamera.unkC8;
    func_ov000_0233e668();
}
