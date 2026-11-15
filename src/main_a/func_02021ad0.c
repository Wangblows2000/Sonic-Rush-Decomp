#include <nitro/types.h>

typedef struct { u8 unk0[0x2C]; u32 unk2C; } UnkStruct_02021ad0;

void func_02021ad0(UnkStruct_02021ad0 *s, u32 v) {
    s->unk2C = v;
}
