#include <nitro/types.h>

typedef struct UnkStruct_0230ba28 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0230ba28;

extern UnkStruct_0230ba28 gCamera;

void MapObject_Destructor(void *p);

void Hand2d_Destructor(void *p) {
    gCamera.unkC8 &= ~0x40000;
    MapObject_Destructor(p);
}
