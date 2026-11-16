#include <nitro/types.h>

typedef struct UnkStruct_02312b30_tp {
    u16 x;
    u16 y;
    u8 pad4[0xc];
    u32 flags;
} UnkStruct_02312b30_tp;

typedef struct UnkStruct_02312b30_p {
    s32 x;
    s32 y;
} UnkStruct_02312b30_p;

typedef struct UnkStruct_02312b30 {
    u8 pad0[0x134];
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    s32 unk140;
} UnkStruct_02312b30;

extern UnkStruct_02312b30_tp data_022b65b4;
extern s32 data_ov001_023178d8[];

s32 func_ov001_0231293c(s32 x, s32 y);

void func_ov001_02312b30(UnkStruct_02312b30 *p) {
    UnkStruct_02312b30_p v = {0, 0};

    if (!(data_022b65b4.flags & 0x10))
        return;
    p->unk134 = func_ov001_0231293c(data_022b65b4.x, data_022b65b4.y);
    p->unk13C = 0;
    p->unk140 = 0;
    p->unk138 = 0x800;
    if (p->unk134 == data_ov001_023178d8[0] || p->unk134 == data_ov001_023178d8[2])
        p->unk140 = 1;
}
