#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
} UnkStruct_023623e0_Obj;

typedef struct {
    u8 pad0[0x850];
    UnkStruct_023623e0_Obj obj[19];
    u8 padB94[0x1028 - 0xB94];
    void *unk1028;
} UnkStruct_023623e0;

void func_02040ef0(UnkStruct_023623e0_Obj *o, void *res, s32 a);

#pragma thumb on

void func_ov000_023623e0(UnkStruct_023623e0 *w) {
    void *res = w->unk1028;

    func_02040ef0(&w->obj[0], res, 0);
    func_02040ef0(&w->obj[1], res, 1);
    func_02040ef0(&w->obj[2], res, 2);
    func_02040ef0(&w->obj[3], res, 3);
    func_02040ef0(&w->obj[4], res, 4);
    func_02040ef0(&w->obj[5], res, 5);
    func_02040ef0(&w->obj[6], res, 7);
    func_02040ef0(&w->obj[7], res, 8);
    func_02040ef0(&w->obj[8], res, 9);
    func_02040ef0(&w->obj[9], res, 10);
    func_02040ef0(&w->obj[10], res, 11);
    func_02040ef0(&w->obj[11], res, 12);
    func_02040ef0(&w->obj[12], res, 13);
    func_02040ef0(&w->obj[13], res, 14);
    func_02040ef0(&w->obj[14], res, 15);
    func_02040ef0(&w->obj[15], res, 16);
    func_02040ef0(&w->obj[16], res, 17);
    func_02040ef0(&w->obj[17], res, 18);
}

#pragma thumb reset
