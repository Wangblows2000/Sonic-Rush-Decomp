#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
    u16 unk48;
    u8 pad4a[0x76];
    s32 timer;
    u8 padc4[4];
} UnkEntry_02041a54;

typedef struct {
    void (*fn)(void);
    u8 pad4[0xe];
    u16 unk12;
    u8 pad14[6];
    u16 unk1a;
    u8 pad1c[0xc];
    UnkEntry_02041a54 entries[32];
    u16 count;
    u16 idx;
} UnkWork_02041a54;

u32 WH_GetSystemState(void);
void WH_ChildConnect(s32 a, UnkEntry_02041a54 *e);
void func_02041a04(void);

void func_02041a54(UnkWork_02041a54 *w) {
    u16 i;

    for (i = 0; i < w->count; i++)
        w->entries[w->count].timer++;
    if (WH_GetSystemState() == 1 && w->idx != 0xffff) {
        w->unk12 = w->entries[w->idx].unk48;
        WH_ChildConnect(5, &w->entries[w->idx]);
        w->fn = func_02041a04;
    }
    w->unk1a = 0;
}
