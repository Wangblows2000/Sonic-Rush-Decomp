#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x454 - 0x6c];
    u16 unk454;
} UnkStruct_022f0b44;

typedef struct {
    u8 pad0[0x20c];
    u32 unk20C;
} UnkStruct_022f0b44_g;

extern UnkStruct_022f0b44_g *data_ov000_02369b84;
extern s32 data_022c4560;
extern u16 data_022c4da0;

void func_020367b4(s32 a, s32 b);
void func_02060f40(void);
void func_ov000_022f0ad0(UnkStruct_022f0b44 *p);

void func_ov000_022f0b44(UnkStruct_022f0b44 *p) {
    if (p->unk454++ <= 0x78)
        return;
    func_020367b4(0, 0x13);
    data_ov000_02369b84->unk20C |= 1;
    if (data_022c4560 == 0)
        data_022c4da0 = 0xf;
    else
        data_022c4da0 = 0x10;
    func_02060f40();
    p->unk68 = func_ov000_022f0ad0;
}
