#include <nitro/types.h>

typedef struct { s32 unk0; u8 *unk4; u16 unk8; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
typedef struct { u8 unk0[8]; u16 unk8; } UnkStruct_02369ac0;
extern UnkTask *data_022b4574;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
void func_02032d2c(void *p);
void func_02044c10(void);
void func_02044ba8(void) { UnkWork *w = data_022b4574->unk10; u8 *p = w->unk4; w->unk0--; if (w->unk0 <= 0) { data_022b4574->fn = func_02044c10; w->unk8 = data_ov000_02369ac0.unk8; } func_02032d2c(p + 0x400); }
