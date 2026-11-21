#include <nitro/types.h>

typedef struct {
    u8 data[0x48];
} UnkFSFile;

typedef struct {
    u32 top;
    u32 size;
    void *dst;
    u32 result;
} UnkReq_02015bdc;

typedef struct {
    u8 pad0[0x10];
    u8 name[4];
    u32 unk14;
} UnkArc_02015bdc;

typedef struct {
    u8 pad0[0x10];
    UnkReq_02015bdc *req;
    UnkArc_02015bdc *arc;
} UnkArg_02015bdc;

void FS_InitFile(UnkFSFile *f);
void *FS_FindArchive(void *name, u32 len);
BOOL FS_OpenFileDirect(UnkFSFile *f, void *arc, u32 top, u32 bottom, s32 id);
s32 FS_ReadFile(UnkFSFile *f, void *dst, u32 size);
void FS_CloseFile(UnkFSFile *f);
void OS_Terminate(void);

void MBi_ReloadCache(UnkArg_02015bdc *arg)
{
    UnkFSFile file;
    UnkArc_02015bdc *arc = arg->arc;
    UnkReq_02015bdc *req = arg->req;
    u32 top;
    FS_InitFile(&file);
    top = req->top;
    if (FS_OpenFileDirect(&file, FS_FindArchive(arc->name, arc->unk14), top, top + req->size, -1)) {
        if (req->size == FS_ReadFile(&file, req->dst, req->size)) {
            req->result = 2;
        }
        FS_CloseFile(&file);
    }
    if (req->result != 2) {
        OS_Terminate();
    }
}
