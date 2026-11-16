#include <nitro/types.h>

typedef struct UnkStruct_0232dec0 {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0xc];
    void (*func)(void);
} UnkStruct_0232dec0;

void func_ov000_0232ddd4(void);

void EffZakoDebris_Action_Init(UnkStruct_0232dec0 *s) {
    s->unk58 |= 0x10;
    s->func = func_ov000_0232ddd4;
}
