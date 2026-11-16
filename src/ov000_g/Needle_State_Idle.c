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

s32 ObjRect_FlagCheck(void *a, void *b);
void Player_OnHurt(void *a, void *b);
void Needle_Action_StartExtendDelay(UnkStruct_02310b00 *w);
void Needle_Action_Slide(UnkStruct_02310b00 *w);

void Needle_State_Idle(UnkStruct_02310b00 *w) {
    UnkStruct_02310b00_R *r;

    if (w->unk1E0->unk2 >= 0x5d) {
        w->unkC++;
        if (w->unkC > 0x78) {
            if (w->unk5C & 0x20) {
                w->unk1F8 = 1;
                Needle_Action_StartExtendDelay(w);
            } else {
                w->unk1F8 = 2;
                Needle_Action_Slide(w);
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
    if (ObjRect_FlagCheck(w->unk154, r->unk2C4) == 0)
        return;
    Player_OnHurt(r->unk2A0, w->unk134);
}
