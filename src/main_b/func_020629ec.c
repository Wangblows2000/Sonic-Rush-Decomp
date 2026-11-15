#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x14]; u32 unk14; } UnkCfg_020629ec;
typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; u8 unk14[0x14]; u8 unk28[0x44]; UnkCfg_020629ec *unk6C; } UnkStruct_020629ec;
void func_02061c58(void *p, u8 v);
void func_020629b8(void);
void func_020629ec(UnkStruct_020629ec *s) { func_02061c58(s->unk28, s->unk6C->unk14); s->unk10 = 2; s->fn = func_020629b8; }

#pragma thumb reset
