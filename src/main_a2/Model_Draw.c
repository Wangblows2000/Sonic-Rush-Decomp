#include <nitro/types.h>

typedef struct UnkObj_020315e0 UnkObj_020315e0;
typedef void (*UnkFn_020315e0)(UnkObj_020315e0 *o);
struct UnkObj_020315e0 {
    u32 type;
    u32 flag;
    u8 unk8[2];
    u8 cb[8];
    u8 unk12[6];
    fx32 trans[3];
    fx32 rot[9];
    fx32 scale[3];
    u8 unk54[0x3C];
    u32 unk90;
    u32 unk94;
    u32 unk98;
    u32 unk9C;
};
typedef struct { u8 unk0[0xFC]; u32 flag; } UnkGlb_020315e0;

extern u8 data_02084df4[];
extern UnkGlb_020315e0 data_02084d38;
extern UnkFn_020315e0 data_02072d08[];

void func_020222bc(const fx32 *trans);
void MI_Copy36B(const void *src, void *dst);
void func_020222e4(const fx32 *scale);
void func_02025268(u32 op, const void *args, u32 n);
void func_02024cfc(void *obj);
void func_02025054(u32 a, u32 b, u32 c, u32 d);
void Model_DrawSprite3D(UnkObj_020315e0 *o);

void Model_Draw(UnkObj_020315e0 *o) {
    u32 a;
    u32 i;
    func_020222bc(o->trans);
    MI_Copy36B(o->rot, data_02084df4);
    data_02084d38.flag &= ~0xA4;
    func_020222e4(o->scale);
    a = 2;
    func_02025268(0x10, &a, 1);
    for (i = 0; i < 8 && o->cb[i] != 0; i++) {
        data_02072d08[o->cb[i]](o);
    }
    if (o->flag & 1) return;
    switch (o->type) {
    case 0:
        func_02024cfc(&o->unk90);
        break;
    case 1:
        func_02025054(o->unk90, o->unk94, o->unk98, o->unk9C);
        break;
    case 2:
        Model_DrawSprite3D(o);
        break;
    }
}
