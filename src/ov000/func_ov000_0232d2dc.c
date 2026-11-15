#include <nitro/types.h>

typedef struct UnkStruct_0232d2dc {
    u8 pad[8];
    s32 unk8;
    s32 unkC;
    u8 pad1[0x46 - 0x10];
    u16 unk46;
    u8 pad2[0x68 - 0x48];
    void (*func)(void);
} UnkStruct_0232d2dc;

void func_ov000_0232d084(void);
void func_0204b7ec(s32 a, s32 b, s32 c);

void func_ov000_0232d2dc(UnkStruct_0232d2dc *s) {
    s->unkC = 0;
    s->unk8 = 0;
    s->unk46 = 0;
    s->func = func_ov000_0232d084;
    func_0204b7ec(0, 0, 0xe7);
}
