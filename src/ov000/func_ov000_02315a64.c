#include <nitro/types.h>

typedef struct UnkStruct_02315a64_2 {
    u8 pad[0x1dc];
    void *unk1DC;
} UnkStruct_02315a64_2;

typedef struct UnkStruct_02315a64 {
    u8 pad[0x10];
    UnkStruct_02315a64_2 *unk10;
} UnkStruct_02315a64;

void func_0201c070(void *p, s32 a);
void func_ov000_022fb9c8(UnkStruct_02315a64 *s);

void func_ov000_02315a64(UnkStruct_02315a64 *s) {
    func_0201c070(s->unk10->unk1DC, 0);
    func_ov000_022fb9c8(s);
}
