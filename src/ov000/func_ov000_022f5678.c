#include <nitro/types.h>

typedef struct UnkStruct_022f5678 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f5678;

void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022f5678 *s);

void func_ov000_022f5678(UnkStruct_022f5678 *s) {
    func_02031768(s->unk10 + 0x470);
    func_ov000_022fb9c8(s);
}
