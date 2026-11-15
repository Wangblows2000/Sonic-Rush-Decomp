#include <nitro/types.h>

typedef struct UnkStruct_02025004 UnkStruct_02025004;
struct UnkStruct_02025004 { u8 *cmd; u32 unk4; u32 flags; };
typedef void (*UnkFunc_02025004)(UnkStruct_02025004 *, u32);
extern UnkFunc_02025004 data_02077c60[];

void func_02025004(UnkStruct_02025004 *s) {
    do {
        u8 c;
        s->flags &= ~0x40;
        c = *s->cmd;
        data_02077c60[c & 0x1F](s, c & 0xE0);
    } while (!(s->flags & 0x20));
}
