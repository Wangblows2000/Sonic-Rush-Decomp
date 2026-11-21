#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3c];
    u16 unk3c;
    u8 pad3e[0x44 - 0x3e];
    u32 unk44;
    u8 pad48[0x50 - 0x48];
    u8 unk50[8];
} UnkStruct_0201b0e8;

void MI_CpuCopy8(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

BOOL CHT_IsPictochatParent(UnkStruct_0201b0e8 *p)
{
    vu16 buf[4];

    if (p == NULL) {
        return FALSE;
    }
    if (p->unk3c == 0) {
        return FALSE;
    }
    MI_CpuCopy8(p->unk50, (void *)buf, 8);
    DC_StoreRange((void *)buf, 8);
    if (p->unk44 == 0 && (buf[0] == 0x2348 || buf[0] == 0xbd8a) && buf[3] == 4) {
        return TRUE;
    }
    return FALSE;
}
