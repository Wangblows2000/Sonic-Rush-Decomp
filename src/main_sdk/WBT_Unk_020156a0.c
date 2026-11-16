#include <nitro/types.h>

extern u8 data_02080604;
void WBT_Unk_020156a0(void) { data_02080604++; if (data_02080604 == 0) data_02080604++; }
