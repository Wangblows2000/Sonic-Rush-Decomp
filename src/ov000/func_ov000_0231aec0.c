#include <nitro/types.h>

typedef struct UnkStruct_0231aec0 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad2[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_0231aec0;

void func_ov000_0231adac(void);

void func_ov000_0231aec0(UnkStruct_0231aec0 *s) {
    s->unkC = 0x3c;
    s->func = func_ov000_0231adac;
}
