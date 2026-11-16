#include <nitro/types.h>

typedef struct UnkStruct_02310144 {
    s32 kind;
    u8 pad4[0x68 - 4];
    s16 type;
} UnkStruct_02310144;

BOOL func_ov001_02310144(UnkStruct_02310144 *p) {
    if (p->kind == 2) {
        switch (p->type) {
        case 9:
        case 10:
            return TRUE;
        }
    }
    return FALSE;
}
