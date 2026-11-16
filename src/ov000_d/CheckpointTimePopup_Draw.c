#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u8 padC[4];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x94 - 0x18];
    u8 unk94[0xC8 - 0x94];
    u32 unkC8;
    u8 padCC[0x110 - 0xCC];
    u32 unk110;
} UnkStruct_0232b1cc;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0232b1cc *work;
} UnkStruct_0232b1cc_Task;

typedef struct {
    u8 pad0[0x24];
    u32 unk24;
} UnkStruct_0232b1cc_4560;

extern UnkStruct_0232b1cc_Task *gTaskCurrent;
extern UnkStruct_0232b1cc_4560 gGameState;

u32 Hud_Unk_ov000_02337650(s32 a);
void StageObj_Draw2DEx(s32 x, s32 y, s32 a, void *w, void *b, s32 c, s32 d);

void CheckpointTimePopup_Draw(void) {
    s16 i;
    u32 s0;
    u32 s1;
    s32 ofs;
    UnkStruct_0232b1cc *w;
    u32 sh;
    u32 d;

    ofs = 0;
    i = 0;
    w = gTaskCurrent->work;
    sh = 0;
    s0 = w->unkC8;
    s1 = w->unk110;
    for (; i < 5; i++) {
        w->unkC8 = s0;
        w->unk110 = s1;
        if (gGameState.unk24 & 8) {
            w->unkC8 = Hud_Unk_ov000_02337650(0);
            w->unk110 = Hud_Unk_ov000_02337650(1);
        }
        d = (w->unk8 & (0xf << sh)) >> sh;
        w->unkC8 += d * 0x80;
        w->unk110 += d * 0x80;
        StageObj_Draw2DEx(w->unk10 - ofs, w->unk14, 0, w, w->unk94, 0, 0);
        ofs += 0xa00;
        if (i == 1) {
            ofs += 0x600;
        }
        if (i == 3) {
            ofs += 0x600;
        }
        sh += 4;
    }
    w->unkC8 = Hud_Unk_ov000_02337650(0);
    w->unk110 = Hud_Unk_ov000_02337650(1);
    w->unkC8 += 0xba0;
    w->unk110 += 0xba0;
    for (ofs = 0x1400, i = 0; i < 2; i++) {
        StageObj_Draw2DEx(w->unk10 - ofs, w->unk14, 0, w, w->unk94, 0, 0);
        ofs += 0x1a00;
    }
    w->unkC8 = s0;
    w->unk110 = s1;
}
