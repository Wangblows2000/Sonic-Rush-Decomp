#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 1;
            u16 b1 : 5;
            u16 b6 : 2;
            u16 b8 : 8;
        } bits;
    } unk20;
    u8 pad22[2];
    struct {
        u16 b0 : 5;
        u16 b5 : 11;
    } unk24;
} UnkStruct_0208922c;

typedef struct {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    UnkStruct_StageObj *unk434;
    u8 pad438[0x450 - 0x438];
    u16 unk450;
    u16 unk452;
    u16 unk454;
    u16 unk456;
    u8 pad458[0x57C - 0x458];
    u8 unk57C;
} UnkStruct_022f0078;

extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Mgr *data_ov000_02369b84;

void func_ov000_022f9188(void);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022eff30(void);

void func_ov000_022f0078(UnkStruct_022f0078 *w) {
    w->base.unk32 = 0;
    w->unk454 = 0;
    w->unk456 = 0;
    func_ov000_022f9188();
    w->unk434->unk54 |= 8;
    w->unk57C |= 1;
    w->unk450 = 0x100;
    Sound_PlaySeqArc(0, 0, 0xc9);
    w->unk452 = 0;
    gDisplayMain.unk20.raw = 0;
    gDisplayMain.unk20.bits.b0 = 1;
    gDisplayMain.unk20.bits.b6 = 2;
    gDisplayMain.unk24.b0 = 0x10;
    data_ov000_02369b84->unk20C |= 8;
    data_022c4560.unk18 |= 1;
    w->base.unk68 = func_ov000_022eff30;
}
