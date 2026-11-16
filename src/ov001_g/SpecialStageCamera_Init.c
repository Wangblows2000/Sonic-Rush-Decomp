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

extern UnkStruct_02315de8 gSpecialStageCamera;
extern UnkStruct_02315de8_v data_ov001_0231f6bc;
extern UnkStruct_02315de8_v data_ov001_0231f6c8;
extern UnkStruct_02315de8_v data_ov001_0231f010;
extern UnkStruct_02315de8_v data_ov001_0231efec;
extern u8 data_ov001_0231f01c[];

void MI_CpuFill8(void *dest, u8 data, u32 size);
void SpecialStageCamera_CreateList(s32 n);
s32 SpecialStageCamera_Register(void *p);
void SpecialStageCamera_SetPosition(s32 h, UnkStruct_02315de8_v v);

void SpecialStageCamera_Init(void) {
    UnkStruct_02315de8_v a = data_ov001_0231f010;
    UnkStruct_02315de8_v b = data_ov001_0231efec;

    MI_CpuFill8(&gSpecialStageCamera, 0, 0x3c);
    SpecialStageCamera_CreateList(1);
    gSpecialStageCamera.unk0 = 9;
    gSpecialStageCamera.unk4 = 1;
    gSpecialStageCamera.unk8 = 1;
    gSpecialStageCamera.unkC = 0.1f;
    gSpecialStageCamera.unk10 = a;
    gSpecialStageCamera.unk1C = b;
    gSpecialStageCamera.unk28 = 0x1000;
    gSpecialStageCamera.unk34 = SpecialStageCamera_Register(data_ov001_0231f01c);
    SpecialStageCamera_SetPosition(gSpecialStageCamera.unk34, a);
}
