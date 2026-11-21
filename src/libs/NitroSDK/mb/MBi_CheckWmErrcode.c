#include <nitro/types.h>

typedef struct { u8 pad[0x51c]; void (*fn)(u32, void *); } UnkStruct_020182e8;
extern UnkStruct_020182e8 *data_02080f18;
void MBi_CheckWmErrcode(u16 a, s32 b) {
    u16 t[2];
    if (b == 2) return;
    if (b == 0) return;
    t[0] = a;
    t[1] = b;
    data_02080f18->fn(0xff, t);
}
