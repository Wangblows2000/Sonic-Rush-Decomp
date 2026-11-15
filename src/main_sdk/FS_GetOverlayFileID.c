#include <nitro/types.h>

typedef struct { void *arc; u32 id; } FSFileID;
typedef struct { u8 pad[0x18]; u32 file_id; } UnkStruct_0200c828;
extern u8 data_0207efd4[];
FSFileID FS_GetOverlayFileID(UnkStruct_0200c828 *p) { FSFileID r; r.arc = data_0207efd4; r.id = p->file_id; return r; }
