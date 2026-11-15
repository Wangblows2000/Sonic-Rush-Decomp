#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0xc]; s16 unk10; } UnkStruct_02059fd8;
void func_02059fa4(void);
void func_02059fd8(UnkStruct_02059fd8 *s) { s->unk10--; if (s->unk10 <= 0) s->fn = func_02059fa4; }
