#include <nitro/types.h>

typedef struct { s32 x; s32 y; s32 z; } UnkVec_022d5738;

typedef struct {
    UnkVec_022d5738 pos;
    UnkVec_022d5738 up;
    UnkVec_022d5738 at;
} UnkCam_022d5738;

typedef struct { u8 pad0[0x7c]; u32 unk7C; } UnkStruct_02084db8;

extern UnkVec_022d5738 data_02084f78;
extern UnkVec_022d5738 data_02084f84;
extern UnkVec_022d5738 data_02084f90;
extern u8 data_02084d84[];
extern UnkStruct_02084db8 data_02084db8;

void func_020044f4(UnkVec_022d5738 *pos, UnkVec_022d5738 *up, UnkVec_022d5738 *at, s32 a, void *mtx);

#pragma thumb on
void func_ov001_022d5738(void) {
    UnkCam_022d5738 c;

    c.pos.x = 0;
    c.pos.y = 0;
    c.pos.z = 0;
    c.up.x = 0;
    c.up.y = 0x1000;
    c.up.z = 0;
    c.at.x = 0;
    c.at.y = 0;
    c.at.z = -0x32000;
    data_02084f78 = c.pos;
    data_02084f84 = c.up;
    data_02084f90 = c.at;
    func_020044f4(&c.pos, &c.up, &c.at, 0, data_02084d84);
    data_02084db8.unk7C &= ~0xe8;
}
#pragma thumb reset
