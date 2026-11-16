#include <nitro/types.h>

typedef struct UnkStruct_0233efb0 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0233efb0;

extern void *data_ov000_0236ac1c;
extern UnkStruct_0233efb0 gCamera;
extern u32 gSystemFlags;

void func_ov000_0233eed8(void *p);
void func_ov000_0233ed3c(void *p, s32 a);
void func_ov000_0233ec68(void *p);

void func_ov000_0233efb0(void) {
    void *p = data_ov000_0236ac1c;
    func_ov000_0233eed8(p);
    if (gCamera.unkC8 & 1) {
        func_ov000_0233ed3c(p, 0);
        func_ov000_0233ec68(p);
    } else {
        func_ov000_0233ed3c(p, 0);
        func_ov000_0233ed3c(p, 1);
    }
    gSystemFlags |= 0x3000;
}
