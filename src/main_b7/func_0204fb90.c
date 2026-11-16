#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
    s32 unk10;
    u8 pad14[8];
    s32 unk1c;
    s32 unk20;
    s32 unk24;
    u8 pad28[0x3a0 - 0x28];
    u8 unk3a0[0x90];
    u8 unk430[0x48];
    u8 unk478[0x48];
    u8 pad4c0[0x550 - 0x4c0];
    void *files[2];
} UnkWork_0204fb90;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_0204fb90 *work;
} UnkTask_0204fb90;

extern UnkTask_0204fb90 *data_022b4574;
void func_0204f200(UnkWork_0204fb90 *w, void *a, void *b, u16 idx, s32 d);
void func_0204fb18(void);

#pragma thumb on
void func_0204fb90(void) {
    UnkWork_0204fb90 *w = data_022b4574->work;
    void *b = w->files[1];
    void *a = w->unk430;
    switch (w->unk24) {
    case 0:
        w->unkc = 0;
        break;
    case 1:
        w->unkc = 0;
        break;
    case 2:
        w->unkc = 6;
        break;
    case 3:
    default:
        w->unkc = -1;
        break;
    }
    if (w->unkc >= 0)
        func_0204f200(w, a, b, w->unkc, 1);
    data_022b4574->fn = func_0204fb18;
}
#pragma thumb reset
