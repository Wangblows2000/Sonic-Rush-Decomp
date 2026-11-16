#include <nitro/types.h>

typedef struct UnkStruct_022ec530 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022ec530;

void Sound_FreeHandle(void *p);
void Model_Release(void *p);
void func_ov000_022fb9c8(UnkStruct_022ec530 *s);

void func_ov000_022ec530(UnkStruct_022ec530 *s) {
    u8 *p = s->unk10;
    Sound_FreeHandle(*(void **)(p + 0x384));
    Model_Release(p + 0x20c);
    func_ov000_022fb9c8(s);
}
