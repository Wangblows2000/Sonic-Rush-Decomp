#include <nitro/types.h>

typedef struct UnkStruct_0235182c UnkStruct_0235182c;
struct UnkStruct_0235182c {
    u8 unk0;
    u8 unk1;
    u8 pad2[8 - 2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2A - 0x18];
    s16 unk2A;
    u8 pad2C[0x2E - 0x2C];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*state)(UnkStruct_0235182c *w);
    u8 pad6C[0x324 - 0x6C];
    u8 unk324;
    u8 pad325[0x32C - 0x325];
    u32 unk32C;
    u8 pad330[0x334 - 0x330];
    void (*unk334)(UnkStruct_0235182c *w);
    u8 pad338[0x384 - 0x338];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_0235182c *other;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
};

void func_ov000_0234739c(UnkStruct_0235182c *w);
void func_ov000_0234a064(UnkStruct_0235182c *w);
s16 ObjSpd_UpSet(s32 v, s32 a, s32 b);
s16 ObjSpd_DownSet(s32 v, s32 a);

void Player_State_Air(UnkStruct_0235182c *w) {
    s32 v;

    func_ov000_0234739c(w);
    if (w->other != NULL) {
        if ((w->unk14 > w->unk3F4 && !(w->other->unk5C & 2)) || (w->unk14 < w->unk3F4 && (w->other->unk5C & 2))) {
            w->unk30 = ObjSpd_UpSet(w->unk30, -0x32, 0x480);
        } else {
            if (w->unk30 < 0) {
                v = w->unk2A;
                if (v < 0)
                    v = -v;
                if (v > 0x200)
                    w->unk30 = ObjSpd_DownSet(w->unk30, 0x68);
            }
            w->unk30 = ObjSpd_UpSet(w->unk30, 0x32, 0x480);
        }
        if (w->unk10 <= w->unk3F0 || w->unk10 >= w->unk3F8)
            w->other = NULL;
    }
    if (w->other != NULL) {
        w->unk3F4 = 0;
        return;
    }
    w->unk58 |= 0x80;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->other = NULL;
    func_ov000_0234a064(w);
    w->unk334(w);
}
