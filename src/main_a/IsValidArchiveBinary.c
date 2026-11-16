#include <nitro/types.h>

typedef struct { u32 magic; u16 bom; u16 version; } UnkHeader_0202146c;

BOOL IsValidArchiveBinary(UnkHeader_0202146c *h) {
    if (h->magic != 0x4352414E) return FALSE;
    if (h->bom != 0xFFFE) return FALSE;
    return h->version == 0x100;
}
