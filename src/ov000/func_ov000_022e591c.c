#include <nitro/types.h>

typedef struct UnkStruct_022e591c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x9f0 - 0x6c];
    u8 unk9F0[0x1118 - 0x9f0];
    s32 unk1118;
} UnkStruct_022e591c;

extern UnkStruct_022e591c *data_ov000_02369b4c;

void func_ov000_022e620c(void);
void func_ov000_022e552c(void);
void func_02021c38(void *p, void (*f)(void), s32 a, s32 b, s32 c);
void func_020366f4(s32 a, s32 b, s32 c);

void func_ov000_022e591c(void) {
    UnkStruct_022e591c *p = data_ov000_02369b4c;
    func_02021c38(p->unk9F0, func_ov000_022e620c, 0, 6, 3);
    p->unk1118 = 0;
    p->func = func_ov000_022e552c;
    func_020366f4(0, 0, 0xd1);
}
