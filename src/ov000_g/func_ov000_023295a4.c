#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x54 - 0x1C];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x68 - 0x5C];
    void *unk68;
    u8 pad6C[0x2B0 - 0x6C];
    s32 unk2B0;
} UnkStruct_023295a4;

extern void *data_ov000_02367b38[];

UnkStruct_023295a4 *BossEffect_Create(void *a, s32 b, s32 c, s32 d, s32 e, s32 f);
void BossArena_ArcToCirclePos(s32 a, s32 b, s32 x, s32 *ox, s32 *oz, u16 *ang);
void func_ov000_0232957c(void);

UnkStruct_023295a4 *func_ov000_023295a4(UnkStruct_023295a4 *p, s32 dx, s32 b, s32 c, u16 k, s32 d, s32 e) {
    u16 ang;
    s32 ox;
    s32 oz;
    UnkStruct_023295a4 *o = BossEffect_Create(data_ov000_02367b38[k], 3, 1, 0, 0, 0);

    o->unk10 = p->unk10 + (dx << 8);
    o->unk14 = (c << 8) + b * 256;
    o->unk58 |= 0x2100;
    o->unk54 |= 2;
    o->unk2B0 = b;
    o->unk68 = func_ov000_0232957c;
    BossArena_ArcToCirclePos(d, e, o->unk10 << 4, &ox, &oz, &ang);
    o->unk10 = ox >> 4;
    o->unk18 = oz >> 4;
    return o;
}
