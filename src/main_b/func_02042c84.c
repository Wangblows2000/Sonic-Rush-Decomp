#include <nitro/types.h>

typedef struct { u8 unk0[0x5e]; u16 unk5E; u8 unk60[0x68]; } UnkElem;
typedef struct { u8 mac[6]; u8 unk6[0xc2]; } UnkElemB;
typedef struct { UnkElem arr[32]; u8 pad[0x192a - 32 * 0xc8]; u16 cur; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4364;
u16 func_02042998(void);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void func_02042c84(u16 *o) { UnkWork *w = data_022c4364->unk10; o[0] = w->arr[w->cur].unk5E; o[1] = func_02042998(); MI_CpuCopy8(((UnkElemB *)((u8 *)w + 0x2c))[w->cur].mac, o + 2, 6); }
