#include <nitro/types.h>

typedef struct { u8 pad[8]; u16 x; u16 y; u8 pad2[4]; u32 flags; } UnkStruct_022b65b4;
extern UnkStruct_022b65b4 data_022b65b4;
void **func_ov001_0230c090(void);
void *func_ov001_02310174(void *p, void *a, s32 x, s32 y);
void func_ov001_023122d8(void *p, s32 v);

BOOL func_ov001_02312258(u8 *p) {
    if (*(s32 *)(p + 0x114) == 0) {
        return FALSE;
    }
    if (data_022b65b4.flags & 0x40) {
        *(void **)(p + 0x118) = func_ov001_02310174(p, *func_ov001_0230c090(), data_022b65b4.x, data_022b65b4.y);
        if (*(void **)(p + 0x118) != NULL) {
            func_ov001_023122d8(p, 0x12);
            return TRUE;
        }
    }
    return FALSE;
}
