#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u16 unk10; u16 unk12; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4364;
u16 func_02042998(void);
void func_02043204(u16 *out) { UnkWork *w = data_022c4364->unk10; out[0] = w->unk10; out[1] = func_02042998(); out[2] = (w->unk12 + 1) | 0x8000; }
