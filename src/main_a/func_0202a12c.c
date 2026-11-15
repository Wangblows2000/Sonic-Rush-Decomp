#include <nitro/types.h>

typedef struct { void *fn0; void (*fn4)(void *, void *); } UnkVtbl_0202a12c;
typedef struct { u32 unk0; u32 unk4; UnkVtbl_0202a12c *unk8; } UnkStruct_0202a12c;
extern UnkStruct_0202a12c data_02077d74;

void func_0202a12c(void *a, void *b) {
    data_02077d74.unk8->fn4(a, b);
}
