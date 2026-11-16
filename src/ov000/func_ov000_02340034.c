#include <nitro/types.h>

typedef struct UnkStruct_02340034 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_02340034;

extern void *data_ov000_0236ac1c;
extern UnkStruct_02340034 gCamera;
extern u32 gSystemFlags;

void func_ov000_0233ff54(void *p);
void func_ov000_0233fd18(void *p, s32 a);
void func_ov000_0233fc44(void *p);

void func_ov000_02340034(void) {
    void *p = data_ov000_0236ac1c;
    func_ov000_0233ff54(p);
    if (gCamera.unkC8 & 1) {
        func_ov000_0233fd18(p, 0);
        func_ov000_0233fc44(p);
    } else {
        func_ov000_0233fd18(p, 0);
        func_ov000_0233fd18(p, 1);
    }
    gSystemFlags |= 0x3000;
}
