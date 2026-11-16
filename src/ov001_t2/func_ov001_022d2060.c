#include <nitro/types.h>

typedef struct {
    u8 pad0[0xbbc];
    s32 timer;
    u8 padbc0[4];
    s32 end;
    void *objs[4];
} UnkStruct_022d2060;

extern s32 data_ov001_02316108[];

void *func_ov001_022d0158(s32 i);
void func_ov001_022d2018(UnkStruct_022d2060 *p);
void func_ov001_022d2234(UnkStruct_022d2060 *p, void (*fn)(UnkStruct_022d2060 *));
void func_ov001_022d1ff4(UnkStruct_022d2060 *p);

#pragma thumb on
void func_ov001_022d2060(UnkStruct_022d2060 *p) {
    s32 prev = p->timer++;
    s32 i;

    for (i = 0; i < 4; i++) {
        s32 t = data_ov001_02316108[i] - 0x1d0;
        if (t > prev && t <= p->timer && p->objs[i] == NULL) {
            p->objs[i] = func_ov001_022d0158(i);
        }
    }
    if (p->timer >= p->end) {
        func_ov001_022d2018(p);
        func_ov001_022d2234(p, func_ov001_022d1ff4);
    }
}
#pragma thumb reset
