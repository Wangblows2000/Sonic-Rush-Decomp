#include <nitro/types.h>

typedef struct { u8 unk0[0x14]; void *unk14; void *unk18; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022b4574;
void func_02037134(void);
void func_0205a148(void *a, void *b);
void func_0205a23c(void) { UnkWork *w = data_022b4574->unk10; void *a = w->unk14; void *b = w->unk18; func_02037134(); func_0205a148(a, b); }
