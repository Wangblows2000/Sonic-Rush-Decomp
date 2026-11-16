#include <nitro/types.h>

typedef struct {
    u32 flag;
    fx32 scale[3];
    fx32 invScale[3];
    fx32 unk1C[3];
    fx32 rot[9];
    fx32 trans[3];
} UnkJntAnm_02028d20;

void func_02025268(u32 op, const void *args, u32 n);

void func_02028d20(UnkJntAnm_02028d20 *r) {
    BOOL doTrans = FALSE;
    if (!(r->flag & 4)) doTrans = TRUE;
    if ((r->flag & 0x20) && !(r->flag & 8)) {
        if (doTrans) {
            func_02025268(0x1C, r->trans, 3);
            doTrans = FALSE;
        }
        func_02025268(0x1B, r->invScale, 3);
    }
    if (!(r->flag & 2)) {
        if (doTrans) {
            func_02025268(0x19, r->rot, 0xC);
        } else {
            func_02025268(0x1A, r->rot, 9);
        }
    } else if (doTrans) {
        func_02025268(0x1C, r->trans, 3);
    }
    if (!(r->flag & 1)) func_02025268(0x1B, r->scale, 3);
}
