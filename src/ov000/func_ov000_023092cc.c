#include <nitro/types.h>

typedef struct UnkStruct_023092cc {
    u8 pad[0xc];
    s32 unkC;
    u8 pad2[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_023092cc;

void func_ov000_02309200(void);

void func_ov000_023092cc(UnkStruct_023092cc *s) {
    s->unkC = 0xf;
    s->func = func_ov000_02309200;
}
