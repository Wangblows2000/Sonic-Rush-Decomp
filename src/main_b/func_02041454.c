#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; u8 unk1C[0x1ad2]; u16 unk1AEE; } UnkStruct_02041454;
void func_02035018(u16 v);
void func_02041430(void);
void func_02041454(UnkStruct_02041454 *s) { func_02035018(s->unk1AEE); s->unk1A = 0; s->fn = func_02041430; }
