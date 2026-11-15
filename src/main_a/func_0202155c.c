#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_0202155c;
typedef struct { u8 unk0; u8 count; u8 unk2[4]; u16 subOffset; } UnkDict_0202155c;
typedef struct { u8 unk0[8]; UnkDict_0202155c dict; } UnkStruct_0202155c;
BOOL func_020217e4(void *p, void *a);
BOOL func_02021634(void *p, void *a);

BOOL func_0202155c(UnkStruct_0202155c *s, void *a) {
    u32 i;
    BOOL r = TRUE;
    for (i = 0; i < s->dict.count; i++) {
        UnkSub_0202155c *sub = (UnkSub_0202155c *)((u8 *)&s->dict + s->dict.subOffset);
        u8 *p = (u8 *)s + *(u32 *)(sub->data + sub->elemSize * i);
        r &= func_020217e4(p, a);
        r &= func_02021634(p, a);
    }
    return r;
}
