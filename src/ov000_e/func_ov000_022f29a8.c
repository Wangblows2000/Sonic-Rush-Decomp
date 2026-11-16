#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x424 - 0x6C];
    s32 unk424;
    u8 pad428[0x434 - 0x428];
    u16 unk434;
    u16 unk436;
    u8 pad438[0x458 - 0x438];
    s32 unk458;
    u8 pad45C[0x57C - 0x45C];
    u8 unk57C;
} UnkStruct_022f29a8;

extern s16 data_ov000_0236415c[][5];
extern u8 data_ov000_02363df0[];

UnkStruct_StageObj *func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_02328248(s32 a, s32 b, s32 c, s32 d);
void func_ov000_022f2680(void);
void func_ov000_022f28b4(void);

void func_ov000_022f29a8(UnkStruct_022f29a8 *s) {
    s32 x;
    UnkStruct_StageObj *o;

    if (!(s->unk57C & 2))
        return;
    s->unk436 = 0x40;
    x = (s->unk424 >> 4) - 0x7800;
    o = func_ov000_022fad3c(0xda, x, 0x30000, 1, 0, 0, 0, 0, 0);
    o->unk2E = (o->unk2E * (s32)(u32)data_ov000_0236415c[s->unk458][s->unk434]) >> 12;
    Sound_PlaySeqArc(0, 0, 0xbe);
    if (s->unk434 + 1 != data_ov000_02363df0[s->unk458]) {
        s->unk68 = func_ov000_022f2680;
        return;
    }
    func_ov000_02328248(1, x, 0x30000, 0);
    s->unk68 = func_ov000_022f28b4;
}
