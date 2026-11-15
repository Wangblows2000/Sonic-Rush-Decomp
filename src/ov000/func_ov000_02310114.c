#include <nitro/types.h>

typedef struct UnkStruct_02310114 {
    u8 pad[0x8];
    s32 unk8;
    s32 unkC;
    u8 pad1[0x58];
    void (*func)(void);
} UnkStruct_02310114;

void func_ov000_0230fe50(void);

void func_ov000_02310114(UnkStruct_02310114 *s) {
    s->func = func_ov000_0230fe50;
    s->unk8 = 0;
    s->unkC = 0;
}
