#include <nitro/types.h>

typedef struct { u8 unk0[0x17]; u8 unk17; u8 unk18; } UnkStruct_0202200c;

u32 func_0202200c(const u8 *type, UnkStruct_0202200c *s) {
    switch (*type) {
    case 'M':
        return (s->unk18 * 2 + 0x1C) & ~3;
    case 'J':
    case 'V':
        return (s->unk17 * 2 + 0x1C) & ~3;
    default:
        return 0;
    }
}
