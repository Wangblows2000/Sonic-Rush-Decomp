#include <nitro/types.h>

typedef struct UnkStruct_0231fdec {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_0231fdec;

extern u32 data_022c4560[];

void func_ov000_0231fdec(UnkStruct_0231fdec *s) {
    if (data_022c4560[6] & 1)
        s->unk5C &= ~0x20;
}
