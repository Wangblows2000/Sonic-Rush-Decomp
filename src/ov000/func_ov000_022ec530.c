#include <nitro/types.h>

typedef struct UnkStruct_022ec530 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022ec530;

void func_020365bc(void *p);
void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022ec530 *s);

void func_ov000_022ec530(UnkStruct_022ec530 *s) {
    u8 *p = s->unk10;
    func_020365bc(*(void **)(p + 0x384));
    func_02031768(p + 0x20c);
    func_ov000_022fb9c8(s);
}
