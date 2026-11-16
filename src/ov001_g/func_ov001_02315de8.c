#include <nitro/types.h>

typedef struct UnkStruct_02315de8_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02315de8_v;

typedef struct UnkStruct_02315de8 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    float unkC;
    UnkStruct_02315de8_v unk10;
    UnkStruct_02315de8_v unk1C;
    s32 unk28;
    u8 pad2C[0x34 - 0x2c];
    s32 unk34;
    u8 pad38[4];
} UnkStruct_02315de8;

extern UnkStruct_02315de8 data_ov001_0231f6ac;
extern UnkStruct_02315de8_v data_ov001_0231f6bc;
extern UnkStruct_02315de8_v data_ov001_0231f6c8;
extern UnkStruct_02315de8_v data_ov001_0231f010;
extern UnkStruct_02315de8_v data_ov001_0231efec;
extern u8 data_ov001_0231f01c[];

void MI_CpuFill8(void *dest, u8 data, u32 size);
void func_ov001_02314e7c(s32 n);
s32 func_ov001_02314c84(void *p);
void func_ov001_02314ae0(s32 h, UnkStruct_02315de8_v v);

void func_ov001_02315de8(void) {
    UnkStruct_02315de8_v a = data_ov001_0231f010;
    UnkStruct_02315de8_v b = data_ov001_0231efec;

    MI_CpuFill8(&data_ov001_0231f6ac, 0, 0x3c);
    func_ov001_02314e7c(1);
    data_ov001_0231f6ac.unk0 = 9;
    data_ov001_0231f6ac.unk4 = 1;
    data_ov001_0231f6ac.unk8 = 1;
    data_ov001_0231f6ac.unkC = 0.1f;
    data_ov001_0231f6ac.unk10 = a;
    data_ov001_0231f6ac.unk1C = b;
    data_ov001_0231f6ac.unk28 = 0x1000;
    data_ov001_0231f6ac.unk34 = func_ov001_02314c84(data_ov001_0231f01c);
    func_ov001_02314ae0(data_ov001_0231f6ac.unk34, a);
}
