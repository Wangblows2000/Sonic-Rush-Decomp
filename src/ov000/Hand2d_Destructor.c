#include <nitro/types.h>

typedef struct UnkStruct_0230ba28 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0230ba28;

extern UnkStruct_0230ba28 data_ov000_0236aad8;

void MapObject_Destructor(void *p);

void Hand2d_Destructor(void *p) {
    data_ov000_0236aad8.unkC8 &= ~0x40000;
    MapObject_Destructor(p);
}
