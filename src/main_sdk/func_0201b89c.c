#include <nitro/types.h>

typedef struct { u8 pad[0x1c]; u8 fader[0x10]; u8 pad1; u8 x2d; u8 x2e; u8 x2f; u8 pad2[4]; u16 x34; u8 pad3[8]; u16 x3e; u8 x40; u8 x41; } UnkStruct_0201b89c;
void NNSi_SndFaderInit(void *f);
void func_020205a0(void *f, s32 v, u32 c);
void func_0201b89c(UnkStruct_0201b89c *p) {
    p->x2e = 0; p->x2d = 0; p->x2f = 0; p->x34 = 0; p->x3e = 0;
    p->x40 = 0x7f; p->x41 = 0x7f;
    NNSi_SndFaderInit(p->fader);
    func_020205a0(p->fader, 0x7f00, 1);
}
