#include <nitro/types.h>

typedef struct UnkStruct_022e9b4c {
    u8 pad[0x3d8];
    u16 unk3D8;
    u8 pad1[0x66];
    void (*func)(void);
} UnkStruct_022e9b4c;

void func_ov000_022e9798(void);

BOOL func_ov000_022e9b4c(UnkStruct_022e9b4c *s) {
    if (s->unk3D8 & 0x30) {
        s->func = func_ov000_022e9798;
        return TRUE;
    }
    return FALSE;
}
