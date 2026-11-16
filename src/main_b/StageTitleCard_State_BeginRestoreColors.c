#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x456]; u16 unk8CE; } UnkStruct_0205aa80;
void func_ov000_023375d8(void);
void StageTitleCard_State_RestoreColors(void);
void StageTitleCard_State_BeginRestoreColors(UnkStruct_0205aa80 *s) { if (gGameState.unk1C != 8 || gGameState.unk1D != 2) func_ov000_023375d8(); s->unk8CE = 0; s->fn = StageTitleCard_State_RestoreColors; }
