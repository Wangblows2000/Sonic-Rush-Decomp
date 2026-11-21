#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 FSi_GetPackedName(void);
typedef struct UnkStruct_0200b828 { u32 id; struct UnkStruct_0200b828 *next; } UnkStruct_0200b828;
extern UnkStruct_0200b828 *data_0207efa8;
UnkStruct_0200b828 *FS_FindArchive(void) {
    u32 id = FSi_GetPackedName();
    u32 e = OS_DisableInterrupts();
    UnkStruct_0200b828 *p = data_0207efa8;
    while (p != 0 && p->id != id) p = p->next;
    OS_RestoreInterrupts(e);
    return p;
}
