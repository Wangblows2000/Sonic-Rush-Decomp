#include <nitro/types.h>

typedef struct UnkStruct_022d5744 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad2[0x68 - 0x30];
    void (*func)(void);
} UnkStruct_022d5744;

void func_ov000_022d5740(void);

void func_ov000_022d5744(UnkStruct_022d5744 *s) {
    s->unk2E = 0;
    s->func = func_ov000_022d5740;
}
