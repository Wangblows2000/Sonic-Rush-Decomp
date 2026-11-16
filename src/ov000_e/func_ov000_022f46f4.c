#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x21C];
    void *unk21C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x328 - 0x20c];
    s16 unk328;
} UnkStruct_Player;

extern UnkStruct_Player *data_ov000_0236ac60;
extern UnkStruct_Mgr *data_ov000_02369b84;

BOOL func_ov000_022f8f00(void);
BOOL func_ov000_022f8f38(void);
void func_ov000_022f8df8(UnkStruct_StageObj *s, s32 *x, s32 *y, s32 c);
void func_ov000_0234915c(UnkStruct_Player *p, s16 a, s32 b);
void func_ov000_022f4850(void);

BOOL func_ov000_022f46f4(UnkStruct_StageObj *s) {
    UnkStruct_Player *p = data_ov000_0236ac60;
    s32 x;
    s32 y;

    switch (p->unk328) {
    case 0xf:
    case 0x11:
    case 0x12:
        p->base.unk70 = data_ov000_02369b84->unk21C;
        data_ov000_0236ac60->base.unk5C |= 0x40;
        return FALSE;
    }
    if (!func_ov000_022f8f00()) {
        if (func_ov000_022f8f38() || data_ov000_0236ac60->base.unk10 > 0x2c000) {
            func_ov000_022f8df8(s, &x, &y, 0);
            data_ov000_0236ac60->base.unk10 = x;
            data_ov000_0236ac60->base.unk14 = y;
            func_ov000_0234915c(data_ov000_0236ac60, (0x1a000 - data_ov000_0236ac60->base.unk10) >> 7, -0x800);
            func_ov000_022f8df8(s, &data_ov000_0236ac60->base.unk10, &data_ov000_0236ac60->base.unk14, 0);
            data_ov000_0236ac60->base.unk70 = data_ov000_02369b84->unk21C;
            data_ov000_0236ac60->base.unk5C |= 0x40;
            s->unk1B8 = NULL;
            func_ov000_022f4850();
            return TRUE;
        }
    }
    return FALSE;
}
