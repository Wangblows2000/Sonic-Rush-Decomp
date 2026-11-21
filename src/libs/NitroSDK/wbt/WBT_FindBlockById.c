#include <nitro/types.h>

typedef struct UnkStruct_02015540 { u32 id; u8 pad[0x24]; struct UnkStruct_02015540 *next; } UnkStruct_02015540;
UnkStruct_02015540 *WBT_GetBlockListHead(void);
UnkStruct_02015540 *WBT_FindBlockById(u32 id) { UnkStruct_02015540 *p; for (p = WBT_GetBlockListHead(); p != 0; p = p->next) { if (p->id == id) return p; } return 0; }
