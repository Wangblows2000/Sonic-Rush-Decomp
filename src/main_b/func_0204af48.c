#include <nitro/types.h>

typedef struct { u8 unk0[0xf]; u8 unkF; u8 unk10; u8 unk11[0x37]; u32 unk48; } UnkStruct_0204af48;
void func_0204af48(UnkStruct_0204af48 *s) { if (s->unk48 == 0) s->unk10 = 's'; else s->unk10 = 'b'; }
