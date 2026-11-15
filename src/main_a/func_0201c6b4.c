#include <nitro/types.h>

void func_0201b54c(void *p);

typedef struct { u8 unk0[0x44]; void *unk44; u32 unk48; } UnkStruct_0201c6b4;

void func_0201c6b4(UnkStruct_0201c6b4 *s) {
    if (s->unk44 == NULL) return;
    func_0201b54c(s->unk44);
    s->unk44 = NULL;
    s->unk48 = 0;
}
