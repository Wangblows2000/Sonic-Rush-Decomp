#include <nitro/types.h>

typedef struct { u8 unk0[6]; u16 unk6; u8 unk8[4]; u32 bits[1]; } UnkData_0202845c;
typedef struct { fx32 unk0; u32 unk4; UnkData_0202845c *unk8; } UnkStruct_0202845c;

void func_0202845c(u32 *out, UnkStruct_0202845c *s, u32 idx) {
    UnkData_0202845c *d = s->unk8;
    u32 bit = (s->unk0 >> 12) * d->unk6 + idx;
    *out = d->bits[bit >> 5] & (1 << (bit & 0x1F));
}
