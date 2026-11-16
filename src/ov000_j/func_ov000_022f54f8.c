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

extern UnkStruct_022f54f8_mgr *data_022b4574;
extern s16 data_0206e470[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void func_020316f4(void *cam);
void func_020315e0(void *cam);
void func_02025268(s32 a, s32 *b, s32 c);
void func_02025a94(void *a, s32 b);

void func_ov000_022f54f8(void) {
    s32 a0;
    s32 a1;
    s32 a2;
    s32 a3;
    s32 a4;
    s32 a5;
    UnkStruct_022f54f8 *obj;
    UnkStruct_022f54f8_cam *cam;

    obj = data_022b4574->unk10;
    cam = &obj->cam;
    if (obj->unk5C & 0x20)
        return;
    cam->unk48 = obj->unk10 << 4;
    cam->unk4C = -(obj->unk14 << 4);
    cam->unk50 = obj->unk18 << 4;
    MTX_RotY33_(cam->unk24, data_0206e470[((obj->unk45E & 0xffff) >> 4) * 2], data_0206e470[((obj->unk45E & 0xffff) >> 4) * 2 + 1]);
    func_020316f4(cam);
    func_020315e0(cam);
    a0 = 2;
    func_02025268(0x10, &a0, 1);
    a1 = 0x1e;
    func_02025268(0x14, &a1, 1);
    func_02025a94(obj->unk340, 0);
    a2 = 0x1d;
    func_02025268(0x14, &a2, 1);
    func_02025a94(obj->unk370, 0);
    a3 = 0x1c;
    func_02025268(0x14, &a3, 1);
    func_02025a94(obj->unk3A0, 0);
    a4 = 0x1b;
    func_02025268(0x14, &a4, 1);
    func_02025a94(obj->unk3D0, 0);
    a5 = 0x1a;
    func_02025268(0x14, &a5, 1);
    func_02025a94(obj->unk400, 0);
}
