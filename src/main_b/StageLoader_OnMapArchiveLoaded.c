#include <nitro/types.h>

typedef struct { u8 unk0[0x5c]; u32 unk5C; } UnkStruct_0204aee8;
typedef struct { u8 unk0[0x10]; u16 *unk10; } UnkStruct_0236aabc;
typedef struct { u8 unk0[0xe]; u16 unkE; u16 unk10; } UnkStruct_0236abd8;
extern u32 data_ov000_0236aab8;
extern UnkStruct_0236aabc data_ov000_0236aabc;
extern UnkStruct_0236abd8 data_ov000_0236abd8;
void func_ov000_0233bf8c(void);
void StageLoader_OnMapArchiveLoaded(UnkStruct_0204aee8 *s) { u16 *p; data_ov000_0236aab8 = s->unk5C; func_ov000_0233bf8c(); p = data_ov000_0236aabc.unk10; data_ov000_0236abd8.unkE = p[0] << 6; data_ov000_0236abd8.unk10 = p[1] << 6; data_ov000_0236aab8 = 0; }
