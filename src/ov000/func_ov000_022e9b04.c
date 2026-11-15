#include <nitro/types.h>

typedef struct UnkStruct_022e9b04 {
    u8 pad[0x3d8];
    u16 unk3D8;
    u8 pad1[0x66];
    void (*func)(void);
} UnkStruct_022e9b04;

void func_ov000_022e91d0(void);

BOOL func_ov000_022e9b04(UnkStruct_022e9b04 *s) {
    if (s->unk3D8 & 0x80) {
        s->func = func_ov000_022e91d0;
        return TRUE;
    }
    return FALSE;
}
