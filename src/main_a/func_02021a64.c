#include <nitro/types.h>

void func_02004f34(void);
void func_02004ec0(void *src, u32 offset, u32 size);
void func_02004e68(void);

typedef struct { u8 unk0[0x2C]; u32 unk2C; u16 unk30; u16 unk32; u8 unk34[4]; u32 unk38; } UnkStruct_02021a64;

void func_02021a64(UnkStruct_02021a64 *s, BOOL lock) {
    if (lock) func_02004f34();
    func_02004ec0((u8 *)s + s->unk38, (s->unk2C & 0xFFFF) << 3, s->unk30 << 3);
    s->unk32 |= 1;
    if (!lock) return;
    func_02004e68();
}
