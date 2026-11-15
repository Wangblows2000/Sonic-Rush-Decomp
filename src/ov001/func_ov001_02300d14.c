#include <nitro/types.h>

typedef struct UnkStruct_02300d14 { u8 pad[0x14]; u32 flags; u8 pad2[0x64]; struct UnkStruct_02300d14 *parent; } UnkStruct_02300d14;
typedef struct { u8 pad[0xbc]; UnkStruct_02300d14 *objs[0x2e]; } UnkStruct_0231f530;
extern UnkStruct_0231f530 *data_ov001_0231f530;
BOOL func_ov001_02300be4(void *p);

void func_ov001_02300d14(void) {
    s32 i;
    for (i = 0; i < 0x2e; i++) {
        UnkStruct_02300d14 *o = data_ov001_0231f530->objs[i];
        if (func_ov001_02300be4(o) && !(o->flags & 0x100000) && (o->flags & 0x10)) {
            UnkStruct_02300d14 *par;
            BOOL kill = FALSE;
            for (par = o->parent; par != NULL; par = par->parent) {
                if (par->flags & 8) {
                    kill = TRUE;
                    break;
                }
                if (!(par->flags & 0x10)) {
                    break;
                }
            }
            if (kill) {
                o->flags |= 8;
            }
        }
    }
}
