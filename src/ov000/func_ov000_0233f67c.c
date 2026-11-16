#include <nitro/types.h>

typedef struct UnkStruct_0233f67c {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0233f67c;

extern void *data_ov000_0236ac1c;
extern UnkStruct_0233f67c gCamera;
extern u32 gSystemFlags;

void func_ov000_0233f5a4(void *p);
void func_ov000_0233f408(void *p, s32 a);
void func_ov000_0233f334(void *p);

void func_ov000_0233f67c(void) {
    void *p = data_ov000_0236ac1c;
    func_ov000_0233f5a4(p);
    if (gCamera.unkC8 & 1) {
        func_ov000_0233f408(p, 0);
        func_ov000_0233f334(p);
    } else {
        func_ov000_0233f408(p, 0);
        func_ov000_0233f408(p, 1);
    }
    gSystemFlags |= 0x3000;
}
