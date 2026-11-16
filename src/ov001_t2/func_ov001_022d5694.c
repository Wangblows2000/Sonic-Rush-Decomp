#include <nitro/types.h>

typedef struct { s16 x; s16 y; s16 z; } UnkVec16_022d5694;

typedef struct {
    u8 pad0[0xa8];
    void *obj;
} UnkStruct_022d5694;

void func_020221fc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_02002360(const UnkVec16_022d5694 *src, UnkVec16_022d5694 *dst);
void func_02022284(s32 i, s32 x, s32 y, s32 z);
void func_0202224c(u16 a, u16 b, s32 c);
void func_02025dac(void *o, s32 a, s32 b);
void func_02025e1c(void *o, s32 a, s32 b);

#pragma thumb on
void func_ov001_022d5694(UnkStruct_022d5694 *p, s32 unused, s32 mode) {
    void *obj = p->obj;
    UnkVec16_022d5694 v;

    func_020221fc(1, 0, mode, 0, 0x1f, 0);
    if (mode == 1) {
        v.x = 0xfff;
        v.y = -0x1000;
        v.z = -0x2000;
    } else {
        v.x = -0x1000;
        v.y = -0x1000;
        v.z = -0x2000;
    }
    func_02002360(&v, &v);
    func_02022284(0, v.x, v.y, v.z);
    func_0202224c(0x7fff, 0x6f7b, 1);
    func_02025dac(obj, 0, 0xf);
    func_02025e1c(obj, 0, 0x40);
    func_02025e1c(obj, 0, 0x80);
    func_02025dac(obj, 0, 0xc0);
}
#pragma thumb reset
