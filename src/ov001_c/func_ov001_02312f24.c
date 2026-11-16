#include <nitro/types.h>

typedef struct UnkStruct_02312f24_e {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x20 - 0x18];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x5e - 0x2c];
    u16 unk5E;
    u8 pad60[0x68 - 0x60];
    s16 unk68;
    s16 unk6A;
} UnkStruct_02312f24_e;

typedef struct UnkStruct_02312f24 {
    u8 pad0[0x20];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x134 - 0x2c];
    s32 unk134;
    s32 unk138;
    u8 pad13C[0x148 - 0x13c];
    s32 unk148;
    u8 pad14C[0x150 - 0x14c];
    s16 unk150;
    u8 pad152[0x22c - 0x152];
    void (*unk22C)(void);
} UnkStruct_02312f24;

void func_ov001_023113f8(void);
void func_ov001_023115a4(void);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
void func_ov001_02312a0c(UnkStruct_02312f24 *p, s32 v);
void func_ov001_023122d8(UnkStruct_02312f24 *p, s32 v);
void func_ov001_0230d6f4(s32 v);
void func_ov001_0231599c(s32 v);
void func_ov001_023120ec(UnkStruct_02312f24 *p, UnkStruct_02312f24_e *e);
void func_ov001_02311ec8(UnkStruct_02312f24 *p);

void func_ov001_02312f24(UnkStruct_02312f24 *p, UnkStruct_02312f24_e *e) {
    if (p->unk22C == func_ov001_023113f8 || p->unk22C == func_ov001_023115a4) {
        switch (e->unk68) {
        case 0:
        case 1:
            p->unk148 += e->unk5E;
            Sound_PlaySeqArc(0, 0x10, 3);
            break;
        }
        return;
    }
    switch (e->unk68) {
    case 0:
    case 1:
        p->unk148 += e->unk5E;
        Sound_PlaySeqArc(0, 0x10, 3);
        break;
    case 2:
        func_ov001_02312a0c(p, e->unk6A);
        break;
    case 3:
        p->unk150 = e->unk6A;
        func_ov001_023122d8(p, 9);
        func_ov001_0230d6f4(0x800);
        func_ov001_0231599c(1);
        p->unk20 = e->unk20;
        p->unk24 = e->unk24;
        p->unk28 = e->unk28;
        break;
    case 4:
        func_ov001_023120ec(p, e);
        p->unk134 = e->unk16;
        func_ov001_02311ec8(p);
        p->unk138 = 0x800;
        break;
    case 5:
        func_ov001_023120ec(p, e);
        p->unk134 = e->unk16 - 0x4000;
        func_ov001_02311ec8(p);
        p->unk138 = 0x111;
        break;
    case 6:
        func_ov001_023120ec(p, e);
        p->unk134 = e->unk16 + 0x4000;
        func_ov001_02311ec8(p);
        p->unk138 = 0x111;
        break;
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
        Sound_PlaySeqArc(0, 0x10, 0x15);
        break;
    case 0x17:
        func_ov001_0231599c(6);
        break;
    }
}
