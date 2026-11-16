#include <nitro/types.h>

s32 strcmp(const char *a, const char *b) {
    const u8 *p1 = (const u8 *)a;
    const u8 *p2 = (const u8 *)b;
    u32 c1;
    u32 c2;
    u32 align;
    s32 diff;
    c1 = *p1;
    if ((diff = c1 - *p2) != 0) return diff;
    align = (u32)p1 & 3;
    if (((u32)p2 & 3) != align) goto bytes;
    if (align != 0) {
        if (c1 == 0) return 0;
        for (align = 3 - align; align != 0; align--) {
            c1 = *++p1;
            c2 = *++p2;
            if ((diff = c1 - c2) != 0) return diff;
            if (c1 == 0) return 0;
        }
        p1++;
        p2++;
    }
    c1 = *(const u32 *)p1;
    c2 = *(const u32 *)p2;
    if ((c1 + 0xFEFEFEFF) & 0x80808080) goto tail;
    if (c1 != c2) goto back;
    for (;;) {
        p1 += 4;
        p2 += 4;
        c1 = *(const u32 *)p1;
        c2 = *(const u32 *)p2;
        if ((c1 + 0xFEFEFEFF) & 0x80808080) goto tail;
        if (c1 != c2) break;
    }
back:
    p1--;
    p2--;
    goto bytes;
tail:
    c1 = *p1;
    if ((diff = c1 - *p2) != 0) return diff;
bytes:
    if (c1 == 0) return 0;
    for (;;) {
        c1 = *++p1;
        c2 = *++p2;
        if ((diff = c1 - c2) != 0) return diff;
        if (c1 == 0) return 0;
    }
}
