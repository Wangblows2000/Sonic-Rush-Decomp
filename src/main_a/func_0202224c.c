#include <nitro/types.h>

typedef struct { u8 unk0[0x94]; u32 unk94; } UnkStruct_0202224c;
extern UnkStruct_0202224c data_02084d38;

void func_0202224c(u32 a, u32 b, BOOL c) {
    data_02084d38.unk94 = a | (b << 16) | ((c ? 1 : 0) << 15);
}
