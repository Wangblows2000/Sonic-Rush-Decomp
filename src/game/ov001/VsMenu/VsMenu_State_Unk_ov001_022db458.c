#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022db458;

typedef struct {
    u8 data[0x1c];
    u8 pad1c[0x18];
} UnkEntry_022db458;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[8];
    s32 unk10;
    u8 pad14[0xc];
    s32 unk20;
    u8 cur[0x1c];
    u8 pad40[8];
    s32 unk48;
    u8 obj4c[0x28];
    UnkEntry_022db458 entries[8];
    u8 padx[0xb44 - 0x74 - 8 * 0x34];
    UnkAnim_022db458 anims[14];
} UnkWork_022db458;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022db458 *work;
} UnkTask_022db458;

typedef struct {
    u8 pad0[0x18];
    u16 count;
} UnkInfo_022db458;

extern UnkTask_022db458 *gTaskCurrent;
extern UnkInfo_022db458 gGameLanguage;
extern u16 data_02072cbc[];

void KeyframeAnim_Update(UnkAnim_022db458 *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022db458 *a);
void KeyframeAnim_SetAnim(UnkAnim_022db458 *a, s32 id);
void VsMenu_Unk_ov001_022d9e50(UnkWork_022db458 *w, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9e2c(UnkWork_022db458 *w, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9b3c(UnkWork_022db458 *w, s32 i, s32 x, s32 y);
void VsMenu_Unk_ov001_022da374(UnkWork_022db458 *w);
void VsMenu_Unk_ov001_022da33c(UnkWork_022db458 *w);
void VsMenu_Unk_ov001_022da2cc(UnkWork_022db458 *w, s32 i);
void VsMenu_Unk_ov001_022da288(UnkWork_022db458 *w, s32 i);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void VsMenu_BuildGlyphString(void *p, u16 *tbl, u16 n);
void VsMenu_State_Unk_ov001_022db2fc(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022db458(void) {
    UnkWork_022db458 *w = gTaskCurrent->work;
    s32 i;
    BOOL done;
    UnkAnim_022db458 *a;

    KeyframeAnim_Update(&w->anims[0]);
    KeyframeAnim_Update(&w->anims[1]);
    for (i = 0; i < 8; i++) {
        KeyframeAnim_Update(&w->anims[i + 6]);
    }
    a = &w->anims[0];
    VsMenu_Unk_ov001_022d9e50(w, a->x, a->y);
    a = &w->anims[1];
    VsMenu_Unk_ov001_022d9e2c(w, a->x, a->y);
    for (i = 0; i < 8; i++) {
        a = &w->anims[i + 6];
        VsMenu_Unk_ov001_022d9b3c(w, i, a->x, a->y);
    }
    done = TRUE;
    if (!KeyframeAnim_IsFinished(&w->anims[0])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anims[1])) done = FALSE;
    for (i = 0; i < 8 && done; i++) {
        if (!KeyframeAnim_IsFinished(&w->anims[i + 6])) {
            done = FALSE;
        }
    }
    w->unk0++;
    if (done) {
        w->unk0 = 0;
        w->unk4 = 0;
        VsMenu_Unk_ov001_022da374(w);
        VsMenu_Unk_ov001_022da33c(w);
        VsMenu_Unk_ov001_022da2cc(w, 0);
        VsMenu_Unk_ov001_022da2cc(w, 1);
        VsMenu_Unk_ov001_022da288(w, 0);
        VsMenu_Unk_ov001_022da288(w, 1);
        KeyframeAnim_SetAnim(&w->anims[0], 0);
        KeyframeAnim_SetAnim(&w->anims[1], 1);
        KeyframeAnim_SetAnim(&w->anims[2], 2);
        KeyframeAnim_SetAnim(&w->anims[3], 3);
        KeyframeAnim_SetAnim(&w->anims[4], 4);
        KeyframeAnim_SetAnim(&w->anims[5], 5);
        w->unk20 = 0;
        w->unk48 = 0;
        MIi_CpuCopy32(&w->entries[w->unk10], w->cur, 0x1c);
        VsMenu_BuildGlyphString(w->obj4c, data_02072cbc, gGameLanguage.count);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022db2fc;
    }
}
#pragma thumb reset
