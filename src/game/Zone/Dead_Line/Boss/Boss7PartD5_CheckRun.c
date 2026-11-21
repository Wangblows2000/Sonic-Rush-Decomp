#include <nitro/types.h>

typedef struct UnkStruct_022e9b4c {
    u8 pad[0x3d8];
    u16 unk3D8;
    u8 pad1[0x66];
    void (*func)(void);
} UnkStruct_022e9b4c;

void Boss7PartD5_MoveState_StartRun(void);

BOOL Boss7PartD5_CheckRun(UnkStruct_022e9b4c *s) {
    if (s->unk3D8 & 0x30) {
        s->func = Boss7PartD5_MoveState_StartRun;
        return TRUE;
    }
    return FALSE;
}
