#include <nitro/types.h>

typedef struct { u8 unk0[0x30]; u16 unk30; } UnkStruct_02021ad8;

u32 func_02021ad8(UnkStruct_02021ad8 *s) {
    return s->unk30 << 3;
}
