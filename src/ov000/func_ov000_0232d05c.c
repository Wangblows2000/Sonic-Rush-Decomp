#include <nitro/types.h>

typedef struct UnkStruct_0232d05c {
    u8 pad[0x8];
    s32 unk8;
    s32 unkC;
    u8 pad1[0x36];
    u16 unk46;
    u8 pad2[0x20];
    void (*func)(void);
    u8 pad3[0x20a];
    u16 unk276;
} UnkStruct_0232d05c;

void func_ov000_0232ce78(void);

void func_ov000_0232d05c(UnkStruct_0232d05c *s) {
    s->unkC = 0;
    s->unk8 = 0;
    s->unk46 = 0;
    s->func = func_ov000_0232ce78;
    s->unk276 = 0;
}
