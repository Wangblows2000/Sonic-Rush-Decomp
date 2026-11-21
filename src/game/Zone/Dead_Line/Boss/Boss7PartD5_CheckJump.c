#include <nitro/types.h>

typedef struct UnkStruct_022e9b28 {
    u8 pad[0x3dc];
    u16 unk3DC;
    u8 pad1[0x62];
    void (*func)(void);
} UnkStruct_022e9b28;

void Boss7PartD5_MoveState_StartJump(void);

BOOL Boss7PartD5_CheckJump(UnkStruct_022e9b28 *s) {
    if (s->unk3DC & 1) {
        s->func = Boss7PartD5_MoveState_StartJump;
        return TRUE;
    }
    return FALSE;
}
