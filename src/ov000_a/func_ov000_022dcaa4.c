#include <nitro/types.h>

typedef struct UnkStruct_022dcaa4 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x20c - 0x5e];
    u8 unk20C[0x138];
    u8 unk344[0x138];
    u8 pad2[0x480 - 0x47c];
    s32 unk480;
} UnkStruct_022dcaa4;

typedef struct UnkTask_022dcaa4 {
    u8 pad[0x10];
    UnkStruct_022dcaa4 *unk10;
} UnkTask_022dcaa4;

extern UnkTask_022dcaa4 *data_022b4574;

void func_020316f4(void *a);
void func_020315e0(void *a);

void func_ov000_022dcaa4(void) {
    UnkStruct_022dcaa4 *s = data_022b4574->unk10;
    void *a;
    if (s->unk5C & 0x20)
        return;
    a = s->unk20C;
    func_020316f4(a);
    func_020315e0(a);
    if (s->unk480 == 0)
        return;
    a = s->unk344;
    func_020316f4(a);
    func_020315e0(a);
}
