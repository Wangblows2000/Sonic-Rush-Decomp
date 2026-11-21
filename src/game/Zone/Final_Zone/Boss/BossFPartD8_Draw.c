#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022f54f8_cam;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x5c - 0x1c];
    u16 unk5C;
    u8 pad5E[0x340 - 0x5e];
    u8 unk340[0x30];
    u8 unk370[0x30];
    u8 unk3A0[0x30];
    u8 unk3D0[0x30];
    u8 unk400[0x5e];
    u16 unk45E;
    u8 pad460[0x470 - 0x460];
    UnkStruct_022f54f8_cam cam;
} UnkStruct_022f54f8;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_022f54f8 *unk10;
} UnkStruct_022f54f8_mgr;

extern UnkStruct_022f54f8_mgr *gTaskCurrent;
extern s16 data_0206e470[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void Model_Animate(void *cam);
void Model_Draw(void *cam);
void NNS_G3dGeBufferOP_N(s32 a, s32 *b, s32 c);
void NNS_G3dGetCurrentMtx(void *a, s32 b);

void BossFPartD8_Draw(void) {
    s32 a0;
    s32 a1;
    s32 a2;
    s32 a3;
    s32 a4;
    s32 a5;
    UnkStruct_022f54f8 *obj;
    UnkStruct_022f54f8_cam *cam;

    obj = gTaskCurrent->unk10;
    cam = &obj->cam;
    if (obj->unk5C & 0x20)
        return;
    cam->unk48 = obj->unk10 << 4;
    cam->unk4C = -(obj->unk14 << 4);
    cam->unk50 = obj->unk18 << 4;
    MTX_RotY33_(cam->unk24, data_0206e470[((obj->unk45E & 0xffff) >> 4) * 2], data_0206e470[((obj->unk45E & 0xffff) >> 4) * 2 + 1]);
    Model_Animate(cam);
    Model_Draw(cam);
    a0 = 2;
    NNS_G3dGeBufferOP_N(0x10, &a0, 1);
    a1 = 0x1e;
    NNS_G3dGeBufferOP_N(0x14, &a1, 1);
    NNS_G3dGetCurrentMtx(obj->unk340, 0);
    a2 = 0x1d;
    NNS_G3dGeBufferOP_N(0x14, &a2, 1);
    NNS_G3dGetCurrentMtx(obj->unk370, 0);
    a3 = 0x1c;
    NNS_G3dGeBufferOP_N(0x14, &a3, 1);
    NNS_G3dGetCurrentMtx(obj->unk3A0, 0);
    a4 = 0x1b;
    NNS_G3dGeBufferOP_N(0x14, &a4, 1);
    NNS_G3dGetCurrentMtx(obj->unk3D0, 0);
    a5 = 0x1a;
    NNS_G3dGeBufferOP_N(0x14, &a5, 1);
    NNS_G3dGetCurrentMtx(obj->unk400, 0);
}
