#include <nitro/types.h>

typedef struct { u8 unk0[0x9C]; u32 unk9C; } UnkStruct_020221fc;
extern UnkStruct_020221fc data_02084d38;

void func_020221fc(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    data_02084d38.unk9C = f | (a | (b << 4) | (c << 6)) | (d << 24) | (e << 16);
}
