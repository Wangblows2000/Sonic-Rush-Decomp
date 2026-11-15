#include <nitro/types.h>

typedef struct UnkStruct_02315020_v {
    s32 x, y, z;
} UnkStruct_02315020_v;

typedef struct UnkStruct_02315020_o {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[8];
    UnkStruct_02315020_v pos;
    u8 pad2C[0xe4 - 0x2c];
    UnkStruct_02315020_v unkE4;
} UnkStruct_02315020_o;

typedef struct UnkStruct_02315020 {
    s32 state;
    s32 unk4;
    s32 unk8;
    float unkC;
    UnkStruct_02315020_v pos;
    UnkStruct_02315020_v tgt;
    UnkStruct_02315020_v unk28;
    s32 unk34;
} UnkStruct_02315020;

extern u32 data_0208fe70;
extern UnkStruct_02315020_v data_ov001_0231efe0;
extern UnkStruct_02315020_v data_ov001_0231ef8c;
extern UnkStruct_02315020_v data_ov001_0231efd4;
extern UnkStruct_02315020_v data_ov001_0231efbc;
extern UnkStruct_02315020_v data_ov001_0231efa4;
extern UnkStruct_02315020_v data_ov001_0231eff8;
extern UnkStruct_02315020_v data_ov001_0231efc8;
extern UnkStruct_02315020_v data_ov001_0231ef98;

UnkStruct_02315020_o *func_ov001_0230c0a4(void);
void func_ov001_02314ae0(s32 h, UnkStruct_02315020_v v);
void func_ov001_02314a98(s32 h, UnkStruct_02315020_v v);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov001_02315020(UnkStruct_02315020 *c) {
    UnkStruct_02315020_o *o = func_ov001_0230c0a4();

    switch (c->state) {
    case 0:
        if (o->pos.y > 0) {
            UnkStruct_02315020_v a = data_ov001_0231efe0;
            UnkStruct_02315020_v b = {0, 0, 0};
            b.y = o->pos.y;
            c->pos = a;
            c->tgt = b;
            c->unkC = 0.06f;
        } else {
            UnkStruct_02315020_v b = data_ov001_0231ef8c;
            c->pos = data_ov001_0231efd4;
            c->tgt = b;
            c->unkC = 0.06f;
        }
        c->unk4 = 1;
        c->unk8 = 1;
        break;
    case 1:
        c->pos = o->pos;
        c->pos.x += (o->unk16 < 0x8000) ? 0xa000 : -0xa000;
        c->pos.y += 0x2000;
        c->pos.z += 0xf000;
        c->tgt = o->unkE4;
        c->tgt.y += 0x2000;
        c->unk8 = 1;
        c->unkC = 0.175f;
        c->unk4 = 1;
        break;
    case 2: {
        UnkStruct_02315020_v a = data_ov001_0231efbc;
        UnkStruct_02315020_v b = data_ov001_0231efa4;
        c->unk28.x = 0x1000;
        c->unk28.y = 0;
        c->unk28.z = 0;
        c->pos = a;
        c->tgt = b;
        c->pos.x += Rand() % 0x1000 - 0x800;
        c->pos.y += Rand() % 0x1000 + 0x7800;
        c->pos.z -= 0x8000;
        c->tgt.x += Rand() % 0x1000 - 0x800;
        c->tgt.y += Rand() % 0x1000 - 0x800;
        c->unk4 = 1;
        c->unk8 = 0;
        break;
    }
    case 3: {
        UnkStruct_02315020_v a = data_ov001_0231eff8;
        c->pos = data_ov001_0231efc8;
        c->pos.y += 0x14000;
        c->pos.z += 0x14000;
        c->tgt = a;
        c->unk8 = 1;
        c->unkC = 0.1f;
        break;
    }
    case 4:
        c->pos = o->unkE4;
        c->pos.y += 0x5000;
        c->pos.z += 0x19000;
        c->tgt = o->unkE4;
        c->unk8 = 1;
        c->unkC = 0.03f;
        break;
    case 5:
        c->pos = o->unkE4;
        c->pos.y += 0x5000;
        c->pos.z += 0x19000;
        c->tgt = o->unkE4;
        c->unk8 = 1;
        c->unkC = 0.03f;
        break;
    case 6:
        c->pos = o->unkE4;
        c->pos.y += 0x1000;
        c->pos.z += 0x12000;
        c->tgt = o->unkE4;
        c->unk8 = 1;
        c->unkC = 0.015f;
        break;
    case 7: {
        UnkStruct_02315020_v t;
        c->pos.x = o->unkE4.x;
        c->pos.y = -0xf000;
        c->pos.z = o->unkE4.z + 0x64000;
        c->tgt = o->unkE4;
        c->tgt.z += 0x32000;
        c->unk8 = 1;
        c->unkC = 0.025f;
        t.x = o->unkE4.x;
        t.y = o->unkE4.y + 0x8000;
        t.z = o->unkE4.z + 0x6000;
        func_ov001_02314ae0(c->unk34, t);
        break;
    }
    case 8: {
        UnkStruct_02315020_v a = data_ov001_0231ef98;
        c->pos.x = o->unkE4.x;
        c->pos.y = -0xa000;
        c->pos.z = o->unkE4.z + 0x26000;
        c->tgt = a;
        c->unkC = 0.01f;
        c->unk8 = 1;
        break;
    }
    case 9:
        c->unk28.x = 0;
        c->unk28.y = 0;
        c->unk28.z = 0;
        c->pos.x = 0xf000;
        c->pos.y = o->unkE4.y;
        c->pos.z = o->unkE4.z - 0xa000;
        c->tgt = o->unkE4;
        c->tgt.x += 0x12000;
        func_ov001_02314ae0(c->unk34, c->pos);
        func_ov001_02314a98(c->unk34, c->tgt);
        c->unk8 = 0;
        break;
    case 10:
        c->unk28.x = 0x1000;
        c->unk28.y = 0;
        c->unk28.z = 0;
        c->pos.x = 0x7000;
        c->pos.y = o->unkE4.y;
        c->pos.z = o->unkE4.z - 0xa000;
        c->tgt = o->unkE4;
        c->tgt.x += 0x3000;
        c->tgt.y += 0x1000;
        c->tgt.z += 0x1000;
        c->unk8 = 0;
        break;
    case 11:
        c->unk28.x = 0x200;
        c->unk28.y = 0;
        c->unk28.z = 0;
        c->pos.x = o->unkE4.x;
        c->pos.y = o->unkE4.y + 0x1000;
        c->pos.z = o->unkE4.z + 0xb000;
        c->tgt = o->unkE4;
        c->unk8 = 0;
        break;
    }
}
