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
} UnkWork_0204fb18;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_0204fb18 *work;
} UnkTask_0204fb18;

extern UnkTask_0204fb18 *data_022b4574;
void func_0204f200(UnkWork_0204fb18 *w, void *a, void *b, u16 idx, s32 d);
void func_0204fad8(void);

#pragma thumb on
void func_0204fb18(void) {
    UnkWork_0204fb18 *w = data_022b4574->work;
    void *b = w->files[1];
    void *a = w->unk478;
    switch (w->unk24) {
    case 0:
        w->unk10 = -1;
        break;
    case 1:
        w->unk10 = 3;
        break;
    case 2:
        w->unk10 = -1;
        break;
    case 3:
    default:
        w->unk10 = -1;
        break;
    }
    if (w->unk10 >= 0)
        func_0204f200(w, a, b, w->unk10, 1);
    data_022b4574->fn = func_0204fad8;
}
#pragma thumb reset
