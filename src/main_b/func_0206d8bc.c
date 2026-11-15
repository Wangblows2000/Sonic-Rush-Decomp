#include <nitro/types.h>

typedef struct { u8 unk0[0xa]; u16 unkA; } UnkStruct_0206d8bc;
extern u16 data_022c5664;
extern u16 data_022c5668;
extern u8 data_022c5660;
void func_0206d8bc(UnkStruct_0206d8bc *a, UnkStruct_0206d8bc *b) { a->unkA = data_022c5664; b->unkA = data_022c5668; data_022c5660 = 1; }
