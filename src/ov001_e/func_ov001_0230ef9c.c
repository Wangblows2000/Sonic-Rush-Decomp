#include <nitro/types.h>

typedef struct UnkStruct_0230ef9c {
    u8 pad0[0x28];
    s32 z;
    u8 pad2C[0x60 - 0x2c];
    u16 unk60;
} UnkStruct_0230ef9c;

void func_ov001_023099c4(void);
void func_ov001_0230f4bc(UnkStruct_0230ef9c *p);

void func_ov001_0230ef9c(UnkStruct_0230ef9c *p) {
    p->unk60 |= 4;
    if (p->z <= 0)
        return;
    func_ov001_023099c4();
    func_ov001_0230f4bc(p);
}
