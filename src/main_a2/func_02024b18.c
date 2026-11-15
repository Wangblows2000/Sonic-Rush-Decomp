#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef BOOL (*UnkVisCb_02024b18)(BOOL *pVisible, void *arg, u32 node);
typedef struct { u8 unk0[0x18]; void *cbArg; UnkVisCb_02024b18 cb; u8 unk20[0x2C]; u32 mask[1]; } UnkObj_02024b18;

void func_02024b18(UnkRS *rs) {
    if (!(rs->flag & 0x200)) {
        int t;
        BOOL skip;
        u32 node;
        rs->curNode = rs->c[1];
        node = rs->curNode;
        rs->flag |= 4;
        rs->pVisible = &rs->visible;
        skip = UnkRS_CbA(rs, 2, &t);
        if (!skip) {
            UnkObj_02024b18 *obj = rs->obj;
            if (!(obj->cbArg != NULL && (obj->mask[node >> 5] & (1 << (node & 0x1F))) &&
                  obj->cb(rs->pVisible, obj->cbArg, node))) {
                *rs->pVisible = rs->c[2] & 1;
            }
        }
        skip = UnkRS_CbB2(rs, 2, &t);
        if (!skip) {
            if (*rs->pVisible) {
                rs->flag |= 1;
            } else {
                rs->flag &= ~1;
            }
        }
        UnkRS_CbC(rs, 2, t);
    }
    rs->c += 3;
}
