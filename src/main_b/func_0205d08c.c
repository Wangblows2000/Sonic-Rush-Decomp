#include <nitro/types.h>

typedef struct { u32 unk0; u8 unk4[4]; s16 unk8; s16 unkA; u8 unkC[0x40]; s32 unk4C; s32 unk50; } UnkSub_0205d08c;
typedef struct { u8 unk0[0x420]; UnkSub_0205d08c sub; } UnkStruct_0205d08c;
void func_02032d2c(void *p);
void func_0205d08c(UnkStruct_0205d08c *s) { UnkSub_0205d08c *p = &s->sub; p->unk8 = p->unk4C >> 12; p->unkA = p->unk50 >> 12; func_02032d2c(p->unk4); }
