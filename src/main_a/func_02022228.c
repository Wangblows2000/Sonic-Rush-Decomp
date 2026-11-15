#include <nitro/types.h>

typedef struct { u8 unk0[0x98]; u32 unk98; } UnkStruct_02022228;
extern UnkStruct_02022228 data_02084d38;

void func_02022228(u32 a, u32 b, BOOL c) {
    data_02084d38.unk98 = a | (b << 16) | ((c ? 1 : 0) << 15);
}
