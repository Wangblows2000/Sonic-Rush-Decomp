#include <nitro/types.h>

typedef struct UnkStruct_022db900 {
    u8 pad[0xbc0];
    s32 unkBC0;
} UnkStruct_022db900;

s32 func_ov000_022db934(void);
void *func_0202f204(void *p);
void *func_0202f24c(void *p, s32 a);
s32 func_0202f148(void *p);

void func_ov000_022db900(UnkStruct_022db900 *s, void *p) {
    s32 v = func_ov000_022db934();
    s->unkBC0 = func_0202f148(func_0202f24c(func_0202f204(p), v));
}
