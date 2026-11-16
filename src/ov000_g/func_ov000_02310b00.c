#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x80 - 1];
    void *unk80;
    u8 pad84[0x2A0 - 0x84];
    u8 unk2A0[0x2C4 - 0x2A0];
    void *unk2C4;
} UnkStruct_02310b00_R;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x5C - 0x10];
    u16 unk5C;
    u8 pad5E[0x134 - 0x5E];
    u8 unk134[0x20];
    void *unk154;
    u8 pad158[0x1BC - 0x158];
    UnkStruct_02310b00_R *unk1BC;
    u8 pad1C0[0x1E0 - 0x1C0];
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x1F8 - 0x1E4];
    s16 unk1F8;
} UnkStruct_02310b00;

s32 func_0206daf4(void *a, void *b);
void func_ov000_023492f0(void *a, void *b);
void func_ov000_02310aa0(UnkStruct_02310b00 *w);
void func_ov000_023109f4(UnkStruct_02310b00 *w);

void func_ov000_02310b00(UnkStruct_02310b00 *w) {
    UnkStruct_02310b00_R *r;

    if (w->unk1E0->unk2 >= 0x5d) {
        w->unkC++;
        if (w->unkC > 0x78) {
            if (w->unk5C & 0x20) {
                w->unk1F8 = 1;
                func_ov000_02310aa0(w);
            } else {
                w->unk1F8 = 2;
                func_ov000_023109f4(w);
            }
            return;
        }
    }
    if (w->unk1E0->unk2 != 0x4d && w->unk1E0->unk2 != 0x5d)
        return;
    r = w->unk1BC;
    if (r == NULL)
        return;
    if (r->unk80 != w)
        return;
    if (r->unk0 != 1)
        return;
    if (func_0206daf4(w->unk154, r->unk2C4) == 0)
        return;
    func_ov000_023492f0(r->unk2A0, w->unk134);
}
