#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    s32 vram;
    u8 pad38[0x10];
} UnkSpr_02055878;

typedef struct {
    u8 pad0[0x10];
    u8 res10[8];
    u8 res18[8];
    UnkSpr_02055878 spr[4];
} UnkWork_02055878;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02055878 *work;
} UnkTask_02055878;

extern u32 data_02087cb8;

void func_020686dc(void *p);
void func_02039bb0(s32 a, s32 b);
void func_02038914(void);
void func_0201bf40(s32 idx, s32 frames);

void func_02055878(UnkTask_02055878 *t) {
    UnkWork_02055878 *w = t->work;
    u16 i;
    s32 k;

    func_020686dc(w->res10);
    func_020686dc(w->res18);
    func_020686dc(w->res18);
    func_020686dc(w->res18);
    for (i = 0; i < 4; i++)
        func_02039bb0(0, w->spr[i].vram);
    if (data_02087cb8 & 8)
        func_02038914();
    for (k = 3; k < 10; k++)
        func_0201bf40(k, 0);
}
