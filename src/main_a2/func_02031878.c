#include <nitro/types.h>

typedef struct {
    u32 type;
    u32 flag;
    u8 unk8[2];
    u8 cb[8];
    u8 unk12[6];
    fx32 scale[3];
    fx32 rot[9];
    u8 unk48[0x18];
    u8 mtx[0x30];
    u8 unk90[0x4C];
    u32 polyAttr;
    u16 diff;
    u16 unkE2;
    u16 amb;
    u16 unkE6;
    u16 spec;
    u16 unkEA;
    u32 unkEC;
    u32 unkF0;
} UnkObj_02031878;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02000dfc(void *m);
void func_0200148c(void *m);
void func_0203363c(void *p, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);

void func_02031878(UnkObj_02031878 *o, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    vu32 z = 0;
    MIi_CpuClear32(z, o, 0xF4);
    o->type = 2;
    o->flag = c;
    o->cb[0] = 3;
    o->scale[0] = o->scale[1] = o->scale[2] = 0x1000;
    func_02000dfc(o->rot);
    func_0200148c(o->mtx);
    func_0203363c(o->unk90, a, b, c, 0, 1, d, 5, e);
    o->polyAttr &= ~0x30;
    o->polyAttr |= 0x80;
    o->polyAttr = (o->polyAttr & ~0x1F0000) | 0x1F0000;
    o->diff = (o->diff & ~0x7FFF) | 0x7FFF;
    o->diff |= 0x8000;
    o->amb = (o->amb & ~0x7FFF) | 0x7FFF;
    o->spec = 0x7FFF;
    o->unkEC = f;
    o->unkF0 = g;
}
