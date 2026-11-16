#include <nitro/types.h>

typedef struct UnkStruct_022c6980_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    s16 unk2E;
    u8 pad1[0x2ce - 0x30];
    s16 unk2CE;
} UnkStruct_022c6980_3;

typedef struct UnkStruct_022c6980_2 {
    u8 unk0;
    u8 pad[0x10 - 1];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    s16 unk2E;
    u8 pad1[0x34 - 0x30];
    s16 unk34;
    s16 unk36;
    u8 pad2[0x58 - 0x38];
    u32 unk58;
    u8 pad3[0x328 - 0x5c];
    s16 unk328;
} UnkStruct_022c6980_2;

typedef struct UnkStruct_022c6980 {
    s32 unk0;
    u8 pad[8];
    s16 unkC;
    u8 pad0[2];
    s16 unk10;
    u8 pad1[6];
    void *unk18;
} UnkStruct_022c6980;

void func_ov000_023491f8(UnkStruct_022c6980_2 *p, s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

void func_ov000_022c6980(UnkStruct_022c6980 *a, UnkStruct_022c6980 *b) {
    UnkStruct_022c6980_2 *p = b->unk18;
    UnkStruct_022c6980_3 *o = a->unk18;
    s32 vel;
    if (p->unk0 != 1)
        return;
    if (p->unk58 & 0x8000)
        vel = p->unk2E;
    else
        vel = p->unk34;
    if (o->unk10 < p->unk10) {
        if (o->unk2E == 0 && o->unk2CE == 0) {
            if (vel > 0)
                return;
            p->unk36 = ((a->unk0 + a->unk10 - b->unkC) << 8) - p->unk10;
            p->unk58 |= 4;
            p->unk2E = 0;
            p->unk34 = 0;
            return;
        }
        if (p->unk10 >= 0x1aa00) {
            if (p->unk328 == 0xf)
                return;
            func_ov000_023491f8(p, 0x180, -0x600);
            return;
        }
        Sound_PlaySeqArc(0, 0, 0x7b);
        p->unk36 = ((a->unk0 + a->unk10 - b->unkC) << 8) - p->unk10;
        if (p->unk58 & 0x8000)
            p->unk2E = 0xa00;
        else
            p->unk34 = 0xa00;
    } else {
        if (o->unk2E == 0 && o->unk2CE == 0) {
            if (vel < 0)
                return;
            p->unk36 = ((a->unk0 + a->unkC - b->unk10) << 8) - p->unk10;
            p->unk58 |= 4;
            p->unk2E = 0;
            p->unk34 = 0;
            return;
        }
        if (p->unk10 <= 0x1800) {
            if (p->unk328 == 0xf)
                return;
            func_ov000_023491f8(p, -0x180, -0x600);
            return;
        }
        Sound_PlaySeqArc(0, 0, 0x7b);
        p->unk36 = ((a->unk0 + a->unkC - b->unk10) << 8) - p->unk10;
        if (p->unk58 & 0x8000)
            p->unk2E = -0xa00;
        else
            p->unk34 = -0xa00;
    }
}
