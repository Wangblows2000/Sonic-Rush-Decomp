#include <nitro/types.h>

typedef struct UnkStruct_02340980 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_02340980;

extern void *data_ov000_0236ac1c;
extern UnkStruct_02340980 data_ov000_0236aad8;
extern u32 data_02087cb8;

void func_ov000_023408a0(void *p);
void func_ov000_023405cc(void *p, s32 a);
void func_ov000_02340388(void *p);

void func_ov000_02340980(void) {
    void *p = data_ov000_0236ac1c;
    func_ov000_023408a0(p);
    if (data_ov000_0236aad8.unkC8 & 1) {
        func_ov000_023405cc(p, 0);
        func_ov000_02340388(p);
    } else {
        func_ov000_023405cc(p, 0);
        func_ov000_023405cc(p, 1);
    }
    data_02087cb8 |= 0x3000;
}
