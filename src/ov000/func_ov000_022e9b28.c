#include <nitro/types.h>

typedef struct UnkStruct_022e9b28 {
    u8 pad[0x3dc];
    u16 unk3DC;
    u8 pad1[0x62];
    void (*func)(void);
} UnkStruct_022e9b28;

void func_ov000_022e93b4(void);

BOOL func_ov000_022e9b28(UnkStruct_022e9b28 *s) {
    if (s->unk3DC & 1) {
        s->func = func_ov000_022e93b4;
        return TRUE;
    }
    return FALSE;
}
