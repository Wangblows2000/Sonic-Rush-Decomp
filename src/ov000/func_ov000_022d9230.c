#include <nitro/types.h>

typedef struct UnkStruct_022d9230 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022d9230;

void Sound_FreeHandle(void *p);
void Model_Release(void *p);
void func_ov000_022fb9c8(UnkStruct_022d9230 *s);

void func_ov000_022d9230(UnkStruct_022d9230 *s) {
    u8 *p = s->unk10;
    Sound_FreeHandle(*(void **)(p + 0x2c0));
    Model_Release(p + 0x2c4);
    func_ov000_022fb9c8(s);
}
