#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x10c - 0x54];
    u8 unk10C;
    u8 unk10D;
} UnkStruct_022ebfc8_cam;

typedef struct UnkStruct_022ebfc8 {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x5c - 0x1c];
    u16 unk5C;
    u8 pad5E[0x20c - 0x5e];
    UnkStruct_022ebfc8_cam cam;
    u8 pad31A[0x348 - 0x20c - sizeof(UnkStruct_022ebfc8_cam)];
    struct UnkStruct_022ebfc8 *unk348;
} UnkStruct_022ebfc8;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_022ebfc8 *unk10;
} UnkStruct_022ebfc8_mgr;

extern UnkStruct_022ebfc8_mgr *gTaskCurrent;
extern s16 data_02071470[];
extern s16 data_0206f470[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void Model_Animate(void *cam);
void Model_Draw(void *cam);

void Boss7PartD7_Draw(void) {
    UnkStruct_022ebfc8 *obj;
    UnkStruct_022ebfc8_cam *cam;

    obj = gTaskCurrent->unk10;
    cam = &obj->cam;
    if (obj->unk5C & 0x20)
        return;
    obj->unk10 = obj->unk348->unk10;
    obj->unk14 = obj->unk348->unk14;
    obj->unk18 = obj->unk348->unk18;
    if (obj->unk348->unk5C & 1)
        obj->unk5C |= 1;
    else
        obj->unk5C &= ~1;
    obj->unk5C &= ~8;
    if (obj->unk5C & 4) {
        cam->unk10C |= 4;
        cam->unk10D |= 4;
    } else {
        cam->unk10C &= ~4;
        cam->unk10D &= ~4;
    }
    if (obj->unk5C & 1)
        MTX_RotY33_(cam->unk24, data_02071470[0], data_02071470[1]);
    else
        MTX_RotY33_(cam->unk24, data_0206f470[0], data_0206f470[1]);
    cam->unk48 = obj->unk10 << 4;
    cam->unk4C = -(obj->unk14 << 4);
    cam->unk50 = obj->unk18 << 4;
    Model_Animate(cam);
    Model_Draw(cam);
    if (cam->unk10C & 2)
        obj->unk5C |= 8;
    if (cam->unk10D & 2)
        obj->unk5C |= 8;
}
