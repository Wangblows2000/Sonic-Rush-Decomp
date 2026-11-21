#include <nitro/types.h>

typedef struct { u32 x0; u8 pad[0xc]; void (*fn)(void *); void *arg; } UnkStruct_0207d3bc;
extern UnkStruct_0207d3bc data_0207d3bc;
void MIi_DMAFastCallback(void) { void (*fn)(void *); void *arg; data_0207d3bc.x0 = 0; fn = data_0207d3bc.fn; arg = data_0207d3bc.arg; if (fn != 0) fn(arg); }
