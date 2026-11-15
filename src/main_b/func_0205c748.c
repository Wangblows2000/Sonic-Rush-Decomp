#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
typedef struct { u8 unk0[0x5c]; u16 unk5C; u8 unk5E[0x12]; u32 unk70; } UnkSprite;
typedef struct { void (*fn)(void); UnkSprite *unk4; u32 unk8; } UnkObj_0205c748;
typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205c748;
void func_0205c748(UnkStruct_0205c748 *s, UnkObj_0205c748 *o) { if (data_022c4560.unk1C != 8 || data_022c4560.unk1D != 2) o->unk4->unk70 = o->unk8; o->unk4->unk5C |= 0x40; s->flags |= 0x400000; }
