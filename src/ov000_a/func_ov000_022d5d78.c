#include <nitro/types.h>

typedef struct UnkStruct_022d5d78_p {
    u8 unk0;
    u8 pad[0x14 - 1];
    s32 unk14;
    u8 pad0[0x38 - 0x18];
    s16 unk38;
    u8 pad1[0x62 - 0x3a];
    u16 unk62;
    u8 pad2[0x17c - 0x64];
    s32 unk17C;
    s32 unk180;
    s32 unk184;
    u8 pad3[0x328 - 0x188];
    s16 unk328;
    u8 pad4[0x32c - 0x32a];
    u32 unk32C;
    u8 pad5[0x3b2 - 0x330];
    s16 unk3B2;
    u8 pad6[0x420 - 0x3b4];
    u16 unk420;
    u16 unk422;
    u16 unk424;
} UnkStruct_022d5d78_p;

typedef struct UnkStruct_022d5d78_o {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad0[0x62 - 0x30];
    u16 unk62;
    u8 pad1[0x20c - 0x64];
    u32 unk20C;
} UnkStruct_022d5d78_o;

typedef struct UnkStruct_022d5d78_h {
    u8 pad[4];
    s32 unk4;
    u8 pad0[0xe - 8];
    s16 unkE;
    u8 pad1[0x12 - 0x10];
    s16 unk12;
    u8 pad2[0x18 - 0x14];
    void *unk18;
} UnkStruct_022d5d78_h;

typedef struct UnkStruct_022d5d78_q {
    u8 pad[0x68];
    void (*unk68)(void);
} UnkStruct_022d5d78_q;

typedef struct UnkStruct_022d5d78_b {
    u8 pad[0x20c];
    u32 unk20C;
    s32 unk210;
    UnkStruct_022d5d78_q *unk214;
} UnkStruct_022d5d78_b;

typedef struct UnkStruct_022d5d78_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022d5d78_7;

extern UnkStruct_022d5d78_7 data_ov000_02369ac0;
extern UnkStruct_022d5d78_b *data_ov000_02369b14;
extern UnkStruct_022d5d78_p *data_ov000_0236ac60;

void func_ov000_022d836c(void);
void func_ov000_022daa94(UnkStruct_022d5d78_o *o);
void func_ov000_0234aa80(UnkStruct_022d5d78_p *p);
void func_ov000_023285d0(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_023492f0(UnkStruct_022d5d78_h *a, UnkStruct_022d5d78_h *b);

void func_ov000_022d5d78(UnkStruct_022d5d78_h *a, UnkStruct_022d5d78_h *b) {
    UnkStruct_022d5d78_p *p = b->unk18;
    UnkStruct_022d5d78_o *o = a->unk18;
    if (p->unk0 != 1)
        return;
    if (p->unk328 == 0x30) {
        if (data_ov000_02369ac0.unk4 == 0xe) {
            data_ov000_02369b14->unk214->unk68 = func_ov000_022d836c;
            p->unk62 = 4;
        } else if (data_ov000_02369ac0.unk4 == 0x10) {
            p->unk62 = 4;
            o->unk62 = 4;
            if ((o->unk20C & 1) && !(o->unk20C & 0x40)) {
                o->unk20C |= 0x40;
                if (o->unk2E > 0)
                    o->unk2E = -0x600;
                else
                    o->unk2E = 0x600;
                o->unk20C &= ~8;
                func_ov000_022daa94(o);
                data_ov000_0236ac60->unk32C |= 0x200000;
                data_ov000_0236ac60->unk424 = 0;
                data_ov000_0236ac60->unk422 = data_ov000_0236ac60->unk424;
                data_ov000_0236ac60->unk420 = data_ov000_0236ac60->unk422;
                data_ov000_02369b14->unk20C |= 4;
                data_ov000_02369b14->unk210 = 1;
            }
        }
        p->unk38 = ((a->unk4 + a->unkE - b->unk12) << 8) - p->unk14;
        func_ov000_0234aa80(p);
        func_ov000_023285d0(1, p->unk17C >> 4, -p->unk180 >> 4, p->unk184 >> 4);
        Sound_PlaySeqArc(0, 0, 0xcb);
    } else if (p->unk3B2 == 0) {
        func_ov000_023492f0(b, a);
    }
}
