#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022f7c00_cam;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x5c - 0x1c];
    u16 unk5C;
    u8 pad5E[0x214 - 0x5e];
    UnkStruct_022f7c00_cam cam;
} UnkStruct_022f7c00;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_022f7c00 *unk10;
} UnkStruct_022f7c00_mgr;

extern UnkStruct_022f7c00_mgr *gTaskCurrent;
extern s16 data_02071470[];
extern s16 data_0206f470[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void Model_Animate(void *cam);
void Model_Draw(void *cam);

void func_ov000_022f7c00(void) {
    UnkStruct_022f7c00 *obj;
    UnkStruct_022f7c00_cam *cam;

    obj = gTaskCurrent->unk10;
    cam = &obj->cam;
    if (obj->unk5C & 0x20)
        return;
    cam->unk48 = obj->unk10 << 4;
    cam->unk4C = -(obj->unk14 << 4);
    cam->unk50 = obj->unk18 << 4;
    if (obj->unk5C & 1)
        MTX_RotY33_(cam->unk24, data_02071470[0], data_02071470[1]);
    else
        MTX_RotY33_(cam->unk24, data_0206f470[0], data_0206f470[1]);
    Model_Animate(cam);
    Model_Draw(cam);
}
