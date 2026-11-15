#include <nitro/types.h>

typedef struct UnkStruct_02308b64 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x58];
    void (*func)(void);
    u8 pad2[0x14c];
    s32 unk1B8;
    u8 pad3[0xc];
    s32 unk1C8;
} UnkStruct_02308b64;

void func_ov000_02308a7c(void);

void func_ov000_02308b64(UnkStruct_02308b64 *s) {
    s->unk1B8 = 0;
    s->unk1C8 = 0;
    s->unkC = 0x3c;
    s->func = func_ov000_02308a7c;
}
