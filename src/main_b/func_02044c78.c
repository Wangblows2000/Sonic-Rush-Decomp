#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u8 *unk10; } UnkTask;
extern UnkTask *data_022b4574;
void func_02032d2c(void *p);
void func_02044c78(void) { func_02032d2c(data_022b4574->unk10 + 0x448); }
