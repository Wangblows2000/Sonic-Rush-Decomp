#include <nitro/types.h>

typedef struct { void (*cb)(void); } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022b4574;
void func_020371c8(void *p);
void func_0204bb44(void) { void (*cb)(void) = data_022b4574->unk10->cb; func_020371c8(data_022b4574); if (cb != NULL) cb(); }
