#include <nitro/types.h>

typedef struct { u8 unk0[0xe]; u16 unkE; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
extern u16 data_022c4dbc;
extern u16 data_022c4db8;
extern u16 data_022c4db4;
void NaviM_Unk_0206488c(void) { data_022c4dbc = 0; data_022c4db8 = 0; data_022c4db4 = gGameState.unkE; }
