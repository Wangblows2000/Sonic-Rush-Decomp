#include <nitro/types.h>

typedef struct UnkStruct_022d9230 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022d9230;

void func_020365bc(void *p);
void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022d9230 *s);

void func_ov000_022d9230(UnkStruct_022d9230 *s) {
    u8 *p = s->unk10;
    func_020365bc(*(void **)(p + 0x2c0));
    func_02031768(p + 0x2c4);
    func_ov000_022fb9c8(s);
}
