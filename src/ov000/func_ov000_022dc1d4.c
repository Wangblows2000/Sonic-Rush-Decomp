#include <nitro/types.h>

typedef struct UnkStruct_022dc1d4 {
    u8 pad[0x3a8];
    s32 unk3A8;
    s32 unk3AC;
    s32 unk3B0;
    s32 unk3B4;
    s32 unk3B8;
    s32 unk3BC;
    void (*func)(void);
    s32 unk3C4;
} UnkStruct_022dc1d4;

void func_ov000_022dc1a8(void);

void func_ov000_022dc1d4(UnkStruct_022dc1d4 *s) {
    s->func = func_ov000_022dc1a8;
    s->unk3C4 = 0;
    s->unk3A8 = 0;
    s->unk3AC = 0x20;
    s->unk3B0 = 0x400;
    s->unk3B4 = 0;
    s->unk3B8 = 0;
    s->unk3BC = 0;
}
