#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022ddc3c;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022ddc3c spr[31];
    u8 pad[0xdb0 - 0x210 - 31 * 0x48];
    void *res4;
    void *res5;
    void *res0;
    void *res1;
    void *res3;
} UnkStruct_022ddc3c;

extern u16 data_ov001_023161a4[];

void func_ov001_022dd890(UnkSpr_022ddc3c *s, void *res, u16 anim);
void func_ov001_022dd8d8(UnkSpr_022ddc3c *s, void *res, u16 anim);

#pragma thumb on
void func_ov001_022ddc3c(UnkStruct_022ddc3c *p) {
    s32 i;
    void *res;

    for (i = 0; i < 31; i++) {
        if (i <= 3) {
            res = p->res0;
        } else if (i <= 8) {
            res = p->res1;
        } else if (i <= 24) {
            res = p->res0;
        } else if (i <= 28) {
            res = p->res3;
        } else if (i <= 29) {
            res = p->res4;
        } else if (i <= 30) {
            res = p->res5;
        }
        if (i < 30) {
            func_ov001_022dd890(&p->spr[i], res, data_ov001_023161a4[i]);
        } else {
            func_ov001_022dd8d8(&p->spr[i], res, data_ov001_023161a4[i]);
        }
    }
}
#pragma thumb reset
