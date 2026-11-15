#include <nitro/types.h>

typedef struct UnkStruct_0230d270_2 {
    u8 pad[0x1dc];
    void *unk1DC;
} UnkStruct_0230d270_2;

typedef struct UnkStruct_0230d270 {
    u8 pad[0x10];
    UnkStruct_0230d270_2 *unk10;
} UnkStruct_0230d270;

void func_0201c070(void *p, s32 a);
void func_ov000_022fb9c8(UnkStruct_0230d270 *s);

void func_ov000_0230d270(UnkStruct_0230d270 *s) {
    if (s->unk10->unk1DC != NULL)
        func_0201c070(s->unk10->unk1DC, 0);
    func_ov000_022fb9c8(s);
}
