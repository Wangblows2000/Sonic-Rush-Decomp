#include <nitro/types.h>

typedef struct { u8 pad[0x10]; u8 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
BOOL func_02063750(void);
void func_02031768(void *p);
void func_020371c8(void *p);
void func_020316f4(void *p);
void func_020315e0(void *p);

void func_ov001_022df530(void) {
    u8 *t = data_022b4574->p;
    u8 *q = t + 4;
    if (!func_02063750()) {
        func_02031768(q);
        func_020371c8(data_022b4574);
        return;
    }
    *(s32 *)(t + 0x144) += *(s32 *)(t + 0x14c);
    if (*(s32 *)(t + 0x144) <= -0x12c000) {
        func_02031768(q);
        func_020371c8(data_022b4574);
        return;
    }
    *(s32 *)(q + 0x48) = *(s32 *)(t + 0x140);
    *(s32 *)(q + 0x4c) = *(s32 *)(t + 0x144);
    *(s32 *)(q + 0x50) = *(s32 *)(t + 0x148);
    func_020316f4(q);
    func_020315e0(q);
}
