#include <nitro/types.h>

typedef struct UnkStruct_023107c4 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_023107c4;

void func_02069818(UnkStruct_023107c4 *s, s32 a);
void func_ov000_02310730(void);

void func_ov000_023107c4(UnkStruct_023107c4 *s) {
    func_02069818(s, 0);
    s->unk5C |= 4;
    s->func = func_ov000_02310730;
}
