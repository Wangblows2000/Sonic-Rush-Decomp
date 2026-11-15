#include <nitro/types.h>

typedef struct { u32 unk0; u8 unk4[4]; s16 unk8; s16 unkA; u8 unkC[0x40]; u8 unk4C[4]; s16 unk50; s16 unk52; u8 unk54[0x40]; s32 unk94; s32 unk98; u16 unk9C; } UnkSub_0205d410;
typedef struct { u8 unk0[0xe0]; UnkSub_0205d410 sub; } UnkStruct_0205d410;
void func_02032480(void *p, s32 a, s32 b, u16 c);
void func_0203344c(void *p, s32 a, s32 b);
void func_02032d2c(void *p);
void func_0205d410(UnkStruct_0205d410 *s) { UnkSub_0205d410 *p = &s->sub; p->unk8 = p->unk94 >> 12; p->unkA = p->unk98 >> 12; func_02032480(p->unk4, 0x1000, 0x1000, p->unk9C); p->unk50 = 0; p->unk52 = 0; func_0203344c(p->unk4C, 0, 0); func_02032d2c(p->unk4C); }
