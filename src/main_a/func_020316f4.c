#include <nitro/types.h>

void func_020313cc(void *a, void *b);
void func_020313c8(void *a, void *b);
void func_0203344c(void *a, u32 b, u32 c);

typedef struct { u32 type; u8 unk4[0x8C]; u8 unk90[0x5C]; u32 unkEC; u32 unkF0; } UnkStruct_020316f4;

void func_020316f4(UnkStruct_020316f4 *s) {
    switch (s->type) {
    case 0:
        func_020313cc(s, s);
        break;
    case 1:
        func_020313c8(s, s);
        break;
    case 2:
        func_0203344c(s->unk90, s->unkEC, s->unkF0);
        break;
    }
}
