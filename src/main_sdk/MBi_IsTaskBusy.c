#include <nitro/types.h>

typedef struct { u32 a; u32 flag : 1; } UnkStruct_0201a478;
BOOL MBi_IsTaskBusy(UnkStruct_0201a478 *p) { return p->flag ? 1 : 0; }
