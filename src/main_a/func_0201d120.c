#include <nitro/types.h>

typedef struct { u32 x; u32 y; } UnkPair_0201d120;
typedef struct { u8 unk0[0x78]; UnkPair_0201d120 unk78; } UnkStruct_0201d120;
extern UnkStruct_0201d120 *data_02083f04;

void func_0201d120(UnkPair_0201d120 *out) {
    *out = data_02083f04->unk78;
}
