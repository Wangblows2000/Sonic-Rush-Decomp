#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5c];
    u16 unk5C;
    u8 pad5E[0x228 - 0x5e];
    u8 unk228[0xaf4 - 0x228];
    s32 unkAF4;
    u8 padAF8[0xbd0 - 0xaf8];
    s32 unkBD0;
} UnkStruct_022dec18;

extern UnkStruct_022dec18 *data_ov000_02369b2c;

void func_ov000_022dd068(void);
void func_ov000_022dce70(void);
void Model_Animate(void *cam);
void Model_Draw(void *cam);

void Boss4_Draw_Unk_ov000_022dec18(void) {
    UnkStruct_022dec18 *o = data_ov000_02369b2c;
    void *cam;

    if (o->unk5C & 0x20)
        return;
    o->unkAF4 = 0;
    func_ov000_022dd068();
    if (o->unkBD0 != 0)
        func_ov000_022dce70();
    cam = o->unk228;
    Model_Animate(cam);
    Model_Draw(cam);
}
