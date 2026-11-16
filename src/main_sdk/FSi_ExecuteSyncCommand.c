#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 x8; u8 pad2[4]; u32 x10; u32 x14; } UnkStruct_0200b514;
u32 FSi_TranslateCommand(UnkStruct_0200b514 *p, u32 a);
void FSi_ReleaseCommand(UnkStruct_0200b514 *p, u32 r);
BOOL FSi_NextCommand(u32 a);
void FSi_ExecuteAsyncCommand(void);
BOOL FSi_ExecuteSyncCommand(UnkStruct_0200b514 *p) { FSi_ReleaseCommand(p, FSi_TranslateCommand(p, p->x10)); if (FSi_NextCommand(p->x8)) FSi_ExecuteAsyncCommand(); return p->x14 == 0; }
