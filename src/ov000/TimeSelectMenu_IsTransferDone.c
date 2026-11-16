#include <nitro/types.h>

typedef struct UnkStruct_0235fb7c {
    u8 pad[0x20];
    void *unk20;
} UnkStruct_0235fb7c;

#pragma thumb on
BOOL TimeSelectMenu_IsTransferDone(UnkStruct_0235fb7c *s) {
    if (s->unk20 != NULL)
        return TRUE;
    return FALSE;
}
#pragma thumb reset
