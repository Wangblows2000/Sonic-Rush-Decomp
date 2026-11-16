#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
} UnkStruct_02356538;

UnkStruct_02356538 *func_020514e4(void);
UnkStruct_02356538 *func_02051474(void);

#pragma thumb on

BOOL func_ov000_02356538(void) {
    UnkStruct_02356538 *a = func_020514e4();
    UnkStruct_02356538 *b = func_02051474();

    if (a->unk1 == 0 || b->unk1 == 0) {
        return FALSE;
    }
    if (a->unk0 != b->unk0) {
        return TRUE;
    }
    return FALSE;
}

#pragma thumb reset
